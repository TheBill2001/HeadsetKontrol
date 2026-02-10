// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkconfig.hpp"
#include "hkglobal.hpp"
#include "hkheadsetcontrol.hpp"
#include "hkheadseterror.hpp"
#include "hkkirigamiapplication.hpp"
#include "hklogstorage.hpp"
#include "hkversion.hpp"

#include <QCommandLineParser>
#include <QFile>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickWindow>

// Include last for qApp
#include <QApplication>

#include <KAboutData>
#include <KDBusService>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KNotification>
#include <KStatusNotifierItem>
#include <KWindowSystem>
#include <KirigamiAppDefaults>

using namespace Qt::Literals::StringLiterals;

namespace
{
void raiseWindow(QWindow *window, bool startup = false)
{
    if (window != nullptr) {
        window->show();
        if (startup) {
            KWindowSystem::updateStartupId(window);
        }
        window->raise();
        KWindowSystem::activateWindow(window);
    }
}

void setupStatusNotifierItem(const QPointer<QQuickWindow> &window, QScopedPointer<KStatusNotifierItem> &statusNotifierItem)
{
    if (HKConfig::useTrayIcon()) {
        statusNotifierItem.reset(new KStatusNotifierItem);
        statusNotifierItem->setAssociatedWindow(window);
        statusNotifierItem->setStandardActionsEnabled(false);
        statusNotifierItem->setStatus(KStatusNotifierItem::Active);
        statusNotifierItem->setIconByName(u"headsetkontrol"_s);

        auto *const kirigamiApp = QQmlProperty{window, u"application"_s}.read().value<HKKirigamiApplication *>();
        auto *const menu = statusNotifierItem->contextMenu();
        menu->addSection(i18nc("@title:menu", "Help"));
        menu->addAction(kirigamiApp->aboutAppAction());
        menu->addAction(kirigamiApp->reportBugAction());

        menu->addSection(i18nc("@title:menu", "Control"));
        menu->addAction(kirigamiApp->startAction());
        menu->addAction(kirigamiApp->stopAction());
        menu->addAction(kirigamiApp->refreshAction());

        menu->addSection(i18nc("@title:menu", "Settings"));
        menu->addAction(kirigamiApp->keyBindingsAction());
        menu->addAction(kirigamiApp->configureAction());

        menu->addSeparator();
        menu->addAction(kirigamiApp->quitAction());
    } else {
        statusNotifierItem.reset();
    }
}

void showRunInBackgroundNotification()
{
    auto *const notification = new KNotification("runInBackground"_L1, KNotification::CloseOnTimeout);
    notification->setTitle(i18nc("@title", "Running In Background"));

    if (!HKConfig::useTrayIcon()) {
        notification->setText(i18nc("notification text", "HeadsetKontrol is running in background without tray icon."));
        notification->setIconName("dialog-warning"_L1);

        auto *action = notification->addAction(i18nc("@action notification", "Enable tray icon"));
        QObject::connect(action, &KNotificationAction::activated, HKConfig::self(), [] {
            HKConfig::setUseTrayIcon(true);
        });
    } else {
        notification->setText(i18nc("notification text", "HeadsetKontrol is running in background."));
        notification->setIconName("headsetkontrol"_L1);
    }
    notification->sendEvent();
}

void onHeadsetErrored(const QPointer<QQuickWindow> &window, const QList<HKHeadsetError> &headsetErrors)
{
    if (!headsetErrors.isEmpty()) {
        return;
    }

    QList<HKLogEntry> entries;
    for (const auto &headsetError : headsetErrors) {
        auto entry = HKLogEntry{
            QUuid::createUuid(),
            i18nc("@title headset error", "Error Occurred for %1", headsetError.headsetName()),
            headsetError.capability() == HKHeadset::UnknownCapability
                ? HKUtils::headsetErrorLocaleErrorString(headsetError)
                : i18nc("headset error notification text, %1 is the name of the capability while %2 is the error string",
                        "%1 error: %2.",
                        HKUtils::capabilityToLocaleString(headsetError.capability()),
                        HKUtils::headsetErrorLocaleErrorString(headsetError)),
            headsetError.details(),
            headsetError.timestamp(),
        };

        auto *notification = new KNotification(u"headsetError"_s, KNotification::Persistent);

        notification->setIconName(u"dialog-error"_s);
        notification->setTitle(entry.title());
        notification->setText(entry.message());
        notification->setWindow(window);

        auto *action = notification->addAction(i18nc("@action view error detail", "View detail"));
        QObject::connect(action, &KNotificationAction::activated, [uuid = entry.id(), window] {
            raiseWindow(window);
            QMetaObject::invokeMethod(window, "showError", uuid);
        });

        if (HKConfig::stopOnRefreshError()) {
            action = notification->addAction(i18nc("@action resume update headsetcontrol after errors", "Resume"));
            auto *const startAction = QQmlProperty{window, u"application"_s}.read().value<HKKirigamiApplication *>()->startAction();
            QObject::connect(action, &KNotificationAction::activated, startAction, &QAction::trigger);
        }

        notification->sendEvent();

        entries << std::move(entry);
    }
}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KirigamiAppDefaults::apply(&app);

    KLocalizedString::setApplicationDomain(TRANSLATION_DOMAIN);

    {
        KAboutData aboutData("headsetkontrol"_L1,
                             i18nc("application name", "HeadsetKontrol"),
                             QStringLiteral(HK_VERSION_STRING),
                             i18nc("@item:intext application description", "Control your headsets with HeadsetControl."),
                             KAboutLicense::GPL_V3,
                             u"Trần Nam Tuấn (c) 2026"_s,
                             {},
                             "https://gitlab.com/TheBill2001/HeadsetKontrol"_L1,
                             "https://gitlab.com/TheBill2001/HeadsetKontrol/-/issues"_L1);

        aboutData.addAuthor(i18nc("name of maintainers", "Trần Nam Tuấn"),
                            i18nc("@info:credit", "Maintainer"),
                            "tuantran1632001@gmail.com"_L1,
                            "https://gitlab.com/TheBill2001"_L1,
                            QUrl("https://gitlab.com/uploads/-/system/user/avatar/10312183/avatar.png"_L1));

        // This should be filled in the translation files.
        aboutData.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));

        aboutData.addComponent(i18nc("HeadsetControl name", "HeadsetControl"),
                               i18nc("HeadsetControl description",
                                     "Sidetone and Battery status for Logitech G930, G533, G633, G933 SteelSeries Arctis 7/PRO 2019 and Corsair VOID (Pro) "
                                     "in Linux and MacOSX."),
                               HKHeadsetControl::version(),
                               "https://github.com/Sapd/HeadsetControl"_L1,
                               KAboutLicense::GPL_V3);

        aboutData.addCredit(i18nc("name of HeadsetControl author", "Denis Arnst"),
                            i18nc("HeadsetControl author task", "HeadsetControl author and maintainer"),
                            "git@sapd.eu"_L1,
                            "https://blog.arnst.cc"_L1,
                            QUrl("https://avatars.githubusercontent.com/u/671752"_L1));

        aboutData.setOrganizationDomain("thebill2001.com");
        aboutData.setDesktopFileName(QLatin1StringView(HK_APP_ID));

        KAboutData::setApplicationData(aboutData);
    }

    bool startMinimized = false;
    bool startPaused = false;
    {
        QCommandLineParser parser;

        QCommandLineOption minimizedOption("minimized"_L1, i18nc("@info:shell", "Start application minimized."));
        parser.addOption(minimizedOption);

        QCommandLineOption pausedOption("paused"_L1, i18nc("@info:shell", "Start application with the auto-refresh paused."));
        parser.addOption(minimizedOption);

        KAboutData aboutData = KAboutData::applicationData();
        aboutData.setupCommandLine(&parser);
        parser.process(*qApp);
        aboutData.processCommandLine(&parser);

        startMinimized = parser.isSet(minimizedOption);
        startPaused = parser.isSet(pausedOption);
    }

    KDBusService dbusService(KDBusService::Unique);

    {
        auto *const config = HKConfig::self();
        auto *const client = HKHeadsetControl::instance();

        QObject::connect(config, &HKConfig::runInBackgroundChanged, qApp, [] {
            qApp->setQuitOnLastWindowClosed(!HKConfig::runInBackground());
        });

        QObject::connect(config, &HKConfig::autoStartChanged, &app, [] {
            static const QString autoStartFileName =
                QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/%1.desktop"_L1.arg(QLatin1StringView(HK_APP_ID));
            if (HKConfig::autoStart()) {
                if (QFile::exists(autoStartFileName)) {
                    QFile autoStartFile(autoStartFileName);
                    autoStartFile.remove();
                }

                static const auto resourcePath = ":/%1.desktop"_L1.arg(QLatin1StringView(HK_APP_ID));
                QFile desktopFile(resourcePath);
                desktopFile.copy(autoStartFileName);
            } else {
                QFile autoStartFile(autoStartFileName);
                if (autoStartFile.exists()) {
                    autoStartFile.remove();
                }
            }
        });

        QObject::connect(config, &HKConfig::updateRateChanged, client, [client, config] {
            client->setUpdateRate(config->updateRate());
        });

        QObject::connect(config, &HKConfig::timeoutChanged, client, [client, config] {
            client->setDeviceTimeout(config->timeout());
        });

        QObject::connect(config, &HKConfig::testDeviceEnabledChanged, client, [client, config] {
            client->setTestDeviceEnabled(config->testDeviceEnabled());
        });

        QObject::connect(config, &HKConfig::testProfileChanged, client, [client, config] {
            client->setTestProfile(config->testProfile());
        });

        QObject::connect(config, &HKConfig::stopOnRefreshErrorChanged, client, [client, config] {
            client->setStopOnRefreshError(config->stopOnRefreshError());
        });

        qApp->setQuitOnLastWindowClosed(!HKConfig::runInBackground());

        client->setUpdateRate(config->updateRate());
        client->setDeviceTimeout(config->timeout());
        client->setTestDeviceEnabled(config->testDeviceEnabled());
        client->setTestProfile(config->testProfile());

        if (config->startMinimized()) {
            startMinimized = true;
        }
    }

    QGuiApplication::setWindowIcon(QIcon::fromTheme("headsetkontrol"_L1));

    QQmlApplicationEngine engine;
    KLocalization::setupLocalizedContext(&engine);

    QQuickWindow *mainWindow = nullptr;
    {
        QQmlComponent mainComponent(&engine);
        mainComponent.loadFromModule(HK_APP_ID, "Main"_L1);

        mainWindow = qobject_cast<QQuickWindow *>(mainComponent.createWithInitialProperties({{"visible"_L1, !startMinimized}}, engine.rootContext()));
        if (mainComponent.isError()) {
            const auto errors = mainComponent.errors();
            for (const auto &error : errors) {
                HK_CRITICAL() << error;
            }
        }

        if (mainWindow == nullptr) {
            HK_FATAL("Failed to load main window!");

            return EXIT_FAILURE;
        }
    }

    QObject::connect(mainWindow, &QQuickWindow::closing, &app, [] {
        if (HKConfig::runInBackground()) {
            showRunInBackgroundNotification();
        } else if (!HKConfig::runInBackground() && HKConfig::useTrayIcon()) {
            QCoreApplication::quit();
        }
    });
    if (startMinimized) {
        showRunInBackgroundNotification();
    }

    QObject::connect(&dbusService, &KDBusService::activateRequested, mainWindow, [mainWindow](const QStringList &arguments, const QString &workingDirectory) {
        Q_UNUSED(arguments)
        Q_UNUSED(workingDirectory)
        raiseWindow(mainWindow, true);
    });

    QScopedPointer<KStatusNotifierItem> statusNotifierItem;
    QObject::connect(HKConfig::self(), &HKConfig::useTrayIconChanged, &app, [mainWindow, &statusNotifierItem] {
        setupStatusNotifierItem(mainWindow, statusNotifierItem);
    });
    setupStatusNotifierItem(mainWindow, statusNotifierItem);

    QObject::connect(
        HKHeadsetControl::instance(),
        &HKHeadsetControl::headsetsAdded,
        HKLogStorage::instance(),
        [mainWindow = QPointer(mainWindow)](const QList<HKHeadset *> &headsets) {
            for (auto *headset : headsets) {
                QObject::connect(headset, &HKHeadset::errorsOccurred, HKLogStorage::instance(), [mainWindow](const QList<HKHeadsetError> &headsetErrors) {
                    onHeadsetErrored(mainWindow, headsetErrors);
                });
                onHeadsetErrored(mainWindow, headset->errors());
            }
        });

    if (!startPaused) {
        HKHeadsetControl::start();
    }

    return app.exec();
}
