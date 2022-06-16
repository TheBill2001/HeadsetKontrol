#include <QFile>
#include <QGuiApplication>

#include "appcontroller.h"
#include "headsetkontrolconfig.h"

AppController::AppController(QObject *parent)
    : QObject{parent}
    , m_headsetControl{new HeadsetControl(HeadsetKontrolConfig::self()->binPath(), this)}
{
    // Init tray icon
    if (HeadsetKontrolConfig::self()->runInBackground()) {
        setupTrayIcon();
        QGuiApplication::setQuitOnLastWindowClosed(false);
    }
    connect(HeadsetKontrolConfig::self(), &HeadsetKontrolConfig::RunInBackgroundChanged, this, [this]() {
        if (HeadsetKontrolConfig::self()->runInBackground()) {
            setupTrayIcon();
        } else {
            disconnect(m_trayIcon, &TrayIcon::showWindow, this, &AppController::showWindow);
            disconnect(m_trayIcon, &TrayIcon::showSettings, this, &AppController::showSettings);
            delete m_trayIcon;
            m_trayIcon = nullptr;
        }
        QGuiApplication::setQuitOnLastWindowClosed(!HeadsetKontrolConfig::self()->runInBackground());
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

    // Init remainingTimeUpdateTimer
    m_remainingTimeUpdateTimer.setParent(this);
    m_remainingTimeUpdateTimer.setInterval(0);
    connect(&m_remainingTimeUpdateTimer, &QTimer::timeout, this, &AppController::remainingTimeUpdated);
    connect(config(), &HeadsetKontrolConfig::ShowCountdownTimerChanged, &m_remainingTimeUpdateTimer, [=]() {
        if (config()->showCountdownTimer())
            m_remainingTimeUpdateTimer.start();
        else
            m_remainingTimeUpdateTimer.stop();
    });
    m_remainingTimeUpdateTimer.start();

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

bool AppController::isPaused() const
{
    return !m_timer.isActive();
}

int AppController::remainingTime() const
{
    return m_timer.remainingTime();
}

void AppController::resetHeadsetSettings()
{
    config()->setInactiveTime(config()->defaultInactiveTimeValue());
    config()->setLed(config()->defaultLedValue());
    config()->setVoicePrompt(config()->defaultVoicePromptValue());
    config()->setRotateToMute(config()->defaultRotateToMuteValue());
    config()->setSidetone(config()->defaultSidetoneValue());
    saveSettings();
}

void AppController::saveSettings()
{
    config()->save();
}

void AppController::pauseToggle()
{
    if (isPaused()) {
        m_timer.start();
        m_remainingTimeUpdateTimer.start();
        headsetControl()->queryAll();
    } else {
        m_timer.stop();
        m_remainingTimeUpdateTimer.stop();
    }
    Q_EMIT pauseChanged(isPaused());
}

void AppController::setupTrayIcon()
{
    m_trayIcon = new TrayIcon(this);
    // BUG: QTBUG-53550 cannot use SVG
    m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/apps/headsetkontrol.png")));
    m_trayIcon->show();
    connect(m_trayIcon, &TrayIcon::showWindow, this, &AppController::showWindow);
    connect(m_trayIcon, &TrayIcon::showSettings, this, &AppController::showSettings);
    connect(headsetControl(), &HeadsetControl::batteryChanged, m_trayIcon, [=]() {
        auto battery = headsetControl()->getBattery();
        if (battery == -2)
            m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/apps/headsetkontrol.png")));
        if (battery >= 0 && battery < 10)
            m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/status/headsetkontrol_battery_empty.png")));
        if (battery >= 10 && battery < 35)
            m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/status/headsetkontrol_battery_low.png")));
        if (battery >= 35 && battery < 65)
            m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/status/headsetkontrol_battery_medium.png")));
        if (battery >= 65 && battery < 90)
            m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/status/headsetkontrol_battery_high.png")));
        if (battery >= 90)
            m_trayIcon->setIcon(QIcon(QStringLiteral(":/icons/hicolor/1024x1024/status/headsetkontrol_battery_full.png")));
    });
}

KAboutData AppController::aboutData() const
{
    return KAboutData::applicationData();
}
