#include <QGuiApplication>

#include "appcontroller.h"
#include "headsetkontrolconfig.h"

AppController::AppController(QObject *parent)
    : QObject{parent}
{
    if (HeadsetKontrolConfig::self()->showTrayIcon()) {
        m_trayIcon = new TrayIcon(this);
        m_trayIcon->show();
        connect(m_trayIcon, &TrayIcon::showWindow, this, &AppController::showWindow);
        connect(m_trayIcon, &TrayIcon::showSettings, this, &AppController::showSettings);
        QGuiApplication::setQuitOnLastWindowClosed(false);
    }
    connect(HeadsetKontrolConfig::self(), &HeadsetKontrolConfig::ShowTrayIconChanged, this, [this]() {
        if (HeadsetKontrolConfig::self()->showTrayIcon()) {
            m_trayIcon = new TrayIcon(this);
            m_trayIcon->show();
            connect(m_trayIcon, &TrayIcon::showWindow, this, &AppController::showWindow);
            connect(m_trayIcon, &TrayIcon::showSettings, this, &AppController::showSettings);
        } else {
            disconnect(m_trayIcon, &TrayIcon::showWindow, this, &AppController::showWindow);
            disconnect(m_trayIcon, &TrayIcon::showSettings, this, &AppController::showSettings);
            delete m_trayIcon;
            m_trayIcon = nullptr;
        }
        QGuiApplication::setQuitOnLastWindowClosed(!HeadsetKontrolConfig::self()->showTrayIcon());
    });

    qDebug() << "App controller created: " << this;
}
