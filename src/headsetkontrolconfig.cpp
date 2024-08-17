#include "headsetkontrolconfig.h"

#include <QDir>
#include <QFile>
#include <qapplicationstatic.h>

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

    connect(this, &HeadsetKontrolConfig::AutoStartChanged, this, [&]() {
        QString autoStartFileName = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/autostart/headsetkontrol.desktop");
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
    });
}

HeadsetKontrolConfig::~HeadsetKontrolConfig()
{
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
