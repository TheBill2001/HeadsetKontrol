#include <QDBusConnection>

#include "headsetkontrolapplication.h"

using namespace Qt::Literals::StringLiterals;

HeadsetKontrolApplication::HeadsetKontrolApplication(QObject *parent)
    : QObject{parent}
    , m_trayIcon{new HeadsetKontrolTrayIcon(this)}
{
    setObjectName(u"HeadsetKontrol"_s);

    QDBusConnection::sessionBus().registerService(QStringLiteral("com.gitlab.thebill2001.headsetkontrol"));
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/HeadsetKontrol"), this, QDBusConnection::ExportScriptableContents);

    connect(m_trayIcon, &HeadsetKontrolTrayIcon::showWindow, this, &HeadsetKontrolApplication::showWindow);
    connect(m_trayIcon, &HeadsetKontrolTrayIcon::showSettings, this, &HeadsetKontrolApplication::showSettings);
}

qint64 HeadsetKontrolApplication::pid() const
{
    return QCoreApplication::applicationPid();
}

void HeadsetKontrolApplication::restore()
{
    Q_EMIT showWindow();
}
