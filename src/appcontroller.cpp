#include <QDBusConnection>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QStandardPaths>

#include <KLocalizedString>
#include <KNotification>

#include "appcontroller.h"
#include "headsetkontrolconfig.h"

AppController::AppController(bool startMinimized, QObject *parent)
    : QObject{parent}
    , m_startMinimized{startMinimized}
    , m_headsetControl{new HeadsetControl(HeadsetKontrolConfig::self()->binPath(), this)}
{
    // Init D-Bus
    QDBusConnection::sessionBus().registerService(QStringLiteral("xyz.billsstuff.headsetkontrol"));
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/appController"), this, QDBusConnection::ExportScriptableContents);

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
    m_remainingTimeUpdateTimer.setInterval(1000);
    connect(&m_remainingTimeUpdateTimer, &QTimer::timeout, this, &AppController::remainingTimeUpdated);
    connect(config(), &HeadsetKontrolConfig::ShowCountdownTimerChanged, &m_remainingTimeUpdateTimer, [=]() {
        if (config()->showCountdownTimer())
            m_remainingTimeUpdateTimer.start();
        else
            m_remainingTimeUpdateTimer.stop();
    });
    m_remainingTimeUpdateTimer.start();

    // Init notification
    connect(headsetControl(), &HeadsetControl::batteryChanged, this, [=](int battery) {
        if (!config()->batteryNotification() || battery == -2)
            return;

        static int oldVal = battery;
        if (abs(battery - oldVal) < 10)
            return;
        else
            oldVal = battery;

        auto notification = new KNotification(QStringLiteral("batteryStatus"), KNotification::CloseOnTimeout, this);
        notification->setAutoDelete(true);
        if (battery == -1)
            notification->setText(i18n("Battery is charging."));
        else
            notification->setText(i18n("Battery is at %1%.", battery));
        notification->setIconName(m_trayIcon->icon().name());

        notification->sendEvent();
    });
    connect(headsetControl(), &HeadsetControl::nameChanged, this, [=](const QString &name) {
        if (!config()->deviceChangeNotification())
            return;

        auto notification = new KNotification(QStringLiteral("deviceNameChanged"), KNotification::CloseOnTimeout, this);
        notification->setAutoDelete(true);
        if (name.isEmpty())
            notification->setText(i18n("No known device detected."));
        else
            notification->setText(i18n("Found device %1.", name));
        notification->setIconName(QStringLiteral("headsetkontrol"));

        notification->sendEvent();
    });

    connect(headsetControl(), &HeadsetControl::nameChanged, this, &AppController::applyAllHeadsetSettings);

    connect(config(), &HeadsetKontrolConfig::AutoStartChanged, this, [=]() {
        if (config()->autoStart()) {
            QFile desktopFile(QStringLiteral(":/resources/headsetkontrol.desktop"));

            if (!desktopFile.open(QIODevice::ReadOnly)) {
                qFatal("Cannot open resource file: %s", desktopFile.errorString().toUtf8().constData());
                return;
            }

            auto desktopFileContent = QString::fromUtf8(desktopFile.readAll());
            desktopFileContent.replace(QStringLiteral("Exec=HeadsetKontrol"), QStringLiteral("Exec=HeadsetKontrol --start-minimized"));

            QDir autoStartDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/autostart"));
            autoStartDir.mkdir(QStringLiteral("."));

            QFile autoStartFile(autoStartDir.absolutePath() + QStringLiteral("/headsetkontrol.desktop"));
            autoStartFile.open(QIODevice::WriteOnly);
            autoStartFile.write(desktopFileContent.toUtf8());

            desktopFile.close();
            autoStartFile.close();
        } else {
            QFile autoStartFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/autostart/headsetkontrol.desktop"));
            if (autoStartFile.exists())
                autoStartFile.remove();
        }
    });
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

bool AppController::isStartMinimized() const
{
    return m_startMinimized;
}

void AppController::applyAllHeadsetSettings()
{
    if (headsetControl()->getName().isEmpty())
        return;

    headsetControl()->setInactiveTime(config()->inactiveTime());
    headsetControl()->setLed(config()->led());
    headsetControl()->setVoicePrompt(config()->voicePrompt());
    headsetControl()->setRotateToMute(config()->rotateToMute());
    headsetControl()->setSidetone(config()->sidetone());
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

void AppController::refresh()
{
    m_timer.start();
    m_headsetControl->queryAll();
}

qint64 AppController::getPid()
{
    return QCoreApplication::applicationPid();
}

void AppController::restore()
{
    Q_EMIT showWindow();
}

void AppController::setupTrayIcon()
{
    m_trayIcon = new TrayIcon(this);
    m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol")));
    m_trayIcon->show();
    connect(m_trayIcon, &TrayIcon::showWindow, this, &AppController::showWindow);
    connect(m_trayIcon, &TrayIcon::showSettings, this, &AppController::showSettings);
    connect(headsetControl(), &HeadsetControl::batteryChanged, m_trayIcon, [=]() {
        auto battery = headsetControl()->getBattery();
        if (battery == -2)
            m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol")));
        if (battery >= 0 && battery < 10)
            m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol_battery_empty")));
        if (battery >= 10 && battery < 35)
            m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol_battery_low")));
        if (battery >= 35 && battery < 65)
            m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol_battery_medium")));
        if (battery >= 65 && battery < 90)
            m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol_battery_high")));
        if (battery >= 90)
            m_trayIcon->setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol_battery_full")));
    });
    connect(headsetControl(), &HeadsetControl::queriedChanged, m_trayIcon, [=]() {
        static const QString infoString1(i18n("Device name") + QStringLiteral(": %1"));
        static const QString infoString2(i18n("Chat-mix level") + QStringLiteral(": %1"));
        static const QString infoString3(i18n("Battery") + QStringLiteral(": %1"));

        QStringList infoStrings;

        if (!headsetControl()->getName().isEmpty())
            infoStrings.append(infoString1.arg(headsetControl()->getName()));
        if (headsetControl()->hasChatMixCapabilitiy())
            infoStrings.append(infoString2.arg(headsetControl()->getChatMix()));
        if (headsetControl()->hasBatteryCapability()) {
            auto bat = headsetControl()->getBattery();
            if (bat < -1)
                infoStrings.append(infoString3.arg(i18n("Unavailable")));
            else if (bat < 0)
                infoStrings.append(infoString3.arg(i18n("Charging")));
            else
                infoStrings.append(infoString3.arg(QString::number(bat) + QStringLiteral("%")));
        }

        if (infoStrings.isEmpty())
            m_trayIcon->setToolTip(i18n("HeadsetKontrol"));
        else
            m_trayIcon->setToolTip(infoStrings.join(QStringLiteral("\n")));
    });
}

KAboutData AppController::aboutData() const
{
    return KAboutData::applicationData();
}
