#include <QGuiApplication>

#include "appcontroller.h"
#include "headsetkontrolconfig.h"

AppController::AppController(QObject *parent)
    : QObject{parent}
    , m_headsetControl{new HeadsetControl(HeadsetKontrolConfig::self()->binPath(), this)}
{
    // Init tray icon
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

    // Init timer
    m_timer.setParent(this);
    m_timer.setInterval(config()->updateRate());
    connect(&m_timer, &QTimer::timeout, headsetControl(), &HeadsetControl::queryAll);
    connect(config(), &HeadsetKontrolConfig::UpdateRateChanged, &m_timer, [=]() {
        m_timer.setInterval(config()->updateRate());
        m_timer.start();
        headsetControl()->queryAll();
    });
    m_timer.start();
    headsetControl()->queryAll();

    qDebug() << "App controller created: " << this;
}

HeadsetControl *AppController::headsetControl() const
{
    return m_headsetControl;
}

HeadsetKontrolConfig *AppController::config() const
{
    return HeadsetKontrolConfig::self();
}

KAboutData AppController::aboutData() const
{
    return KAboutData::applicationData();
}
