#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>

#include "headsetcontrol.h"
#include "headsetkontrolconfig.h"
#include "trayicon.h"

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HeadsetControl *headsetControl READ headsetControl CONSTANT)
    Q_PROPERTY(HeadsetKontrolConfig *config READ config CONSTANT)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY pauseChanged)
    Q_PROPERTY(int remainingTime READ remainingTime NOTIFY remainingTimeUpdated)
    Q_PROPERTY(bool startMinimized READ isStartMinimized CONSTANT)
public:
    explicit AppController(bool startMinimizied, QObject *parent = nullptr);

    HeadsetControl *headsetControl() const;
    HeadsetKontrolConfig *config() const;
    bool isPaused() const;
    int remainingTime() const;
    bool isStartMinimized() const;

public Q_SLOTS:
    Q_INVOKABLE void applyAllHeadsetSettings();
    Q_INVOKABLE void resetHeadsetSettings();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE void pauseToggle();
    Q_INVOKABLE void refresh();
    Q_SCRIPTABLE qint64 getPid();
    Q_SCRIPTABLE void restore();

Q_SIGNALS:
    void showWindow();
    void showSettings();
    void pauseChanged(bool paused);
    void remainingTimeUpdated();

private:
    void setupTrayIcon();

    bool m_startMinimized;
    TrayIcon *m_trayIcon;
    HeadsetControl *m_headsetControl;
    QTimer m_timer;
    QTimer m_remainingTimeUpdateTimer;
};

#endif // APPCONTROLLER_H
