#ifndef TRAYICON_H
#define TRAYICON_H

#include <QQmlEngine>
#include <QSystemTrayIcon>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    TrayIcon(QObject *parent = nullptr);

Q_SIGNALS:
    void showWindow();
    void showSettings();
};

#endif // TRAYICON_H
