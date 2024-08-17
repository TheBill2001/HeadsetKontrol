#ifndef HEADSETKONTROLAPPLICATION_H
#define HEADSETKONTROLAPPLICATION_H

#include <QApplication>

#include "headsetkontroltrayicon.h"

class HeadsetKontrolApplication : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(App)
    QML_SINGLETON
public:
    explicit HeadsetKontrolApplication(QObject *parent = nullptr);

    Q_SCRIPTABLE qint64 pid() const;
    Q_SCRIPTABLE void restore();

Q_SIGNALS:
    void showWindow();
    void showSettings();

private:
    HeadsetKontrolTrayIcon *m_trayIcon;
};

#endif // HEADSETKONTROLAPPLICATION_H
