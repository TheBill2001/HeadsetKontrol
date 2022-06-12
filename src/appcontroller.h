#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

#include "trayicon.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);

Q_SIGNALS:
    void showWindow();
    void showSettings();

private:
    TrayIcon *m_trayIcon;
};

#endif // APPCONTROLLER_H
