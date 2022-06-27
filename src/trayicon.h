#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(QObject *parent = nullptr);

Q_SIGNALS:
    void showWindow();
    void showSettings();
};

#endif // TRAYICON_H
