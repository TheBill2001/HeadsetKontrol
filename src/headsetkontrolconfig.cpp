#include "headsetkontrolconfig.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <qapplicationstatic.h>

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;

#ifdef Q_OS_WIN
#define PATH_ENV_SEP ";"_L1
#else
#define PATH_ENV_SEP ":"_L1
#endif

Q_APPLICATION_STATIC(HeadsetKontrolConfig, s_config, QCoreApplication::instance())

HeadsetKontrolConfig::HeadsetKontrolConfig(QObject *parent)
{
    setParent(parent);
    QJSEngine::setObjectOwnership(this, QJSEngine::CppOwnership);

    m_timer.setParent(this);
    m_timer.setInterval(1000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, [=]() {
        if (isSaveNeeded() || config()->isDirty()) {
            save();
        }
    });
    m_timer.start();

    connect(this, &HeadsetKontrolConfig::AutoStartChanged, this, &HeadsetKontrolConfig::checkAutoStart);

    if (executablePath().isEmpty()) {
        QTimer::singleShot(0, this, [&]() {
            qInfo() << i18nc("@info:shell", "Empty HeadsetControl executable path. Attempting to auto-detect...");
            auto pathsString = qEnvironmentVariable("PATH");
            if (pathsString.isEmpty()) {
                qInfo() << i18nc("@info:shell", "PATH environment variable is empty. Cannot auto-detect HeadsetControl executable path.");
            }

            auto paths = pathsString.split(PATH_ENV_SEP);
            for (const auto &path : std::as_const(paths)) {
                QDir dir(path);
                dir.setNameFilters({u"headsetcontrol"_s});

                auto entryList = dir.entryInfoList(QDir::Files | QDir::Executable);

                bool found = false;
                for (const auto &entry : std::as_const(entryList)) {
                    auto filePath = entry.canonicalFilePath();

                    QProcess process(this);
                    process.start(filePath, {u"--connected"_s});
                    auto finished = process.waitForFinished(500);
                    if (finished) {
                        auto reply = QString::fromUtf8(process.readAllStandardOutput()).simplified();
                        if (reply == u"true"_s) {
                            setExecutablePath(entryList.first().canonicalFilePath());
                            found = true;
                            qInfo() << i18nc("@info:shell", "Found HeadsetControl executable path: %1", executablePath());
                        }
                    }
                }

                if (found) {
                    break;
                }
            }
        });
    }

    QTimer::singleShot(0, this, &HeadsetKontrolConfig::checkAutoStart);
}

HeadsetKontrolConfig::~HeadsetKontrolConfig()
{
    m_timer.stop();
    if (isSaveNeeded() || config()->isDirty()) {
        save();
    }
}

HeadsetKontrolConfig *HeadsetKontrolConfig::create(QQmlEngine *, QJSEngine *engine)
{
    Q_UNUSED(engine)
    return instance();
}

KAboutData HeadsetKontrolConfig::aboutData() const
{
    return KAboutData::applicationData();
}

void HeadsetKontrolConfig::setExecutablePathUrl(const QUrl &url)
{
    setExecutablePath(url.toLocalFile());
}

HeadsetKontrolConfig *HeadsetKontrolConfig::instance()
{
    return s_config;
}

void HeadsetKontrolConfig::checkAutoStart()
{
    static QString autoStartFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + u"/autostart/headsetkontrol.desktop"_s;
    if (autoStart()) {
        if (QFile::exists(autoStartFileName)) {
            QFile autoStartFile(autoStartFileName);
            autoStartFile.remove();
        }

        QFile desktopFile(QStringLiteral(":/resources/headsetkontrol.desktop"));
        desktopFile.copy(autoStartFileName);
    } else {
        QFile autoStartFile(autoStartFileName);
        if (autoStartFile.exists())
            autoStartFile.remove();
    }
}
