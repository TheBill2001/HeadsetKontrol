#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

#include <KAboutData>

#include "headsetcontrol.h"
#include "headsetkontrolconfig.h"
#include "trayicon.h"

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT)
    Q_PROPERTY(HeadsetControl *headsetControl READ headsetControl CONSTANT)
    Q_PROPERTY(HeadsetKontrolConfig *config READ config CONSTANT)
public:
    explicit AppController(QObject *parent = nullptr);

    KAboutData aboutData() const;
    HeadsetControl *headsetControl() const;
    HeadsetKontrolConfig *config() const;

Q_SIGNALS:
    void showWindow();
    void showSettings();

private:
    TrayIcon *m_trayIcon;
    HeadsetControl *m_headsetControl;
};

#endif // APPCONTROLLER_H
