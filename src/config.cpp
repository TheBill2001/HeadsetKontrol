#include "config.h"

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
        if (isSaveNeeded()) {
            save();
        }
    });
    m_timer.start();
}

HeadsetKontrolConfig::~HeadsetKontrolConfig()
{
    m_timer.stop();
    if (isSaveNeeded()) {
        save();
    }
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

HeadsetKontrolConfig *HeadsetKontrolConfigQml::create(QQmlEngine *, QJSEngine *engine)
{
    Q_ASSERT_X(engine->thread() == HeadsetKontrolConfig::instance()->thread(), Q_FUNC_INFO, "Accessing HeadsetKontrolConfig in different thread.");

    if (s_engine)
        Q_ASSERT_X(engine == s_engine, Q_FUNC_INFO, "Accessing HeadsetKontrolConfig in different engine.");
    else
        s_engine = engine;

    return HeadsetKontrolConfig::instance();
}
