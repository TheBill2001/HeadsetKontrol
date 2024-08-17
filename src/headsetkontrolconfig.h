#ifndef CONFIG_H
#define CONFIG_H

#include <KAboutData>
#include <QQmlEngine>
#include <QTimer>

#include "headsetkontrolconfigbase.h"

class HeadsetKontrolConfig : public HeadsetKontrolConfigBase
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Config)
    QML_SINGLETON

    Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT FINAL)
public:
    explicit HeadsetKontrolConfig(QObject *parent);
    ~HeadsetKontrolConfig();

    static HeadsetKontrolConfig *create(QQmlEngine *, QJSEngine *engine);

    KAboutData aboutData() const;

    Q_INVOKABLE void setExecutablePathUrl(const QUrl &url);

    static HeadsetKontrolConfig *instance();

private Q_SLOTS:
    void checkAutoStart();

private:
    QTimer m_timer;
    bool m_startMinimized;
};

#endif // CONFIG_H
