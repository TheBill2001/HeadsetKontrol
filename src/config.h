#ifndef CONFIG_H
#define CONFIG_H

#include <KAboutData>
#include <QQmlEngine>
#include <QTimer>

#include "headsetkontrolconfig.h"

class HeadsetKontrolConfig : public HeadsetKontrolConfigBase
{
    Q_OBJECT

    Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT FINAL)
public:
    explicit HeadsetKontrolConfig(QObject *parent);
    ~HeadsetKontrolConfig();

    static HeadsetKontrolConfig *create(QQmlEngine *, QJSEngine *engine);

    KAboutData aboutData() const;

    Q_INVOKABLE void setExecutablePathUrl(const QUrl &url);

    static HeadsetKontrolConfig *instance();

private:
    QTimer m_timer;
};

class HeadsetKontrolConfigQml
{
    Q_GADGET
    QML_FOREIGN(HeadsetKontrolConfig)
    QML_NAMED_ELEMENT(Config)
    QML_SINGLETON
public:
    static HeadsetKontrolConfig *create(QQmlEngine *, QJSEngine *engine);

private:
    inline static QJSEngine *s_engine = nullptr;
};

#endif // CONFIG_H
