#ifndef HEADSETKONTROLTRAYICON_H
#define HEADSETKONTROLTRAYICON_H

#include <QQmlEngine>
#include <QSystemTrayIcon>

class HeadsetKontrolApplication;
class HeadsetKontrolTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    HeadsetKontrolTrayIcon(HeadsetKontrolApplication *parent = nullptr);

Q_SIGNALS:
    void showWindow();
    void showSettings();
};

#endif // HEADSETKONTROLTRAYICON_H
