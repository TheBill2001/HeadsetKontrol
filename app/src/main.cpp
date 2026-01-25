// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "headsetkontrol_version.hpp"
#include "hkconfig.hpp"
#include "hkconfigupdate.hpp"
#include "hkdbusactivationevent.hpp"
#include "hkheadsetcontrol.hpp"
#include "hklogging.hpp"

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtEnvironmentVariables>

#include <KAboutData>
#include <KDBusService>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KirigamiAppDefaults>

using namespace Qt::Literals::StringLiterals;

namespace
{
void checkConfigUpdate()
{
#ifdef Q_OS_LINUX
    // Skip this if we are running in Plasma environment.
    // Is there a more reliable way to achieve this?
    if (qEnvironmentVariable("DESKTOP_SESSION") == "plasma"_L1) {
        return;
    } else {
        const auto config = HKConfig::self()->sharedConfig();
        const auto updates = HKConfigUpdate::updates();
        const QString updateFile = "headsetkontrol.upd"_L1;
        for (const auto &update : updates) {
            config->checkUpdate(update.idString, updateFile);
        }
        config->sync();
        HKConfig::self()->save();
    }
#else
    const auto config = HKConfig::self()->sharedConfig();
    const auto updates = HKConfigUpdate::updates();
    const QString updateFile = "headsetkontrol.upd"_L1;
    auto group = config->group("$Version"_L1);
    QStringList ids = group.readEntry("update_info", QStringList{});

    for (const auto &update : updates) {
        QString cfgId = updateFile + ':'_L1 + update.idString;
        if (!ids.contains(cfgId)) {
            config->checkUpdate(update.idString, updateFile);
            if (update() == EXIT_FAILURE) {
                qCCritical(HK_LOGGING, "Failed to update config ID: %s", qPrintable(update.idString));
            }
            ids.append(std::move(cfgId));
        }
    }
    group.writeEntry("update_info", ids);

    config->sync();
    HKConfig::self()->save();
#endif
}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KirigamiAppDefaults::apply(&app);

    KLocalizedString::setApplicationDomain(TRANSLATION_DOMAIN);

    KAboutData aboutData("headsetkontrol"_L1,
                         "HeadsetKontrol"_L1,
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
                        "https://gitlab.com/TheBill2001"_L1);

    aboutData.setTranslator(i18nc("name of translators", "Trần Nam Tuấn"), u"tuantran1632001@gmail.com"_s);

    aboutData.setOrganizationDomain("thebill2001.com");
    aboutData.setDesktopFileName(QLatin1StringView(HK_APP_ID));

    KAboutData::setApplicationData(aboutData);

    bool startMinimized = false;
    {
        QCommandLineParser parser;

        QCommandLineOption minimizedOption("minimized"_L1, i18nc("@info:shell", "Start application minimized."));
        parser.addOption(minimizedOption);

        aboutData.setupCommandLine(&parser);
        parser.process(app);
        aboutData.processCommandLine(&parser);

        startMinimized = parser.isSet(minimizedOption);
    }

    KDBusService dbusService(KDBusService::Unique);

    QObject::connect(&dbusService, &KDBusService::activateRequested, &app, [&app](const QStringList &arguments, const QString &workingDirectory) {
        QApplication::postEvent(&app, new HKDBusActivateEvent(arguments, workingDirectory), INT_MAX);
    });

    QObject::connect(&dbusService, &KDBusService::activateActionRequested, &app, [&app](const QString &actionName, const QVariant &parameter) {
        QApplication::postEvent(&app, new HKDBusActivateActionEvent(actionName, parameter));
    });

    QObject::connect(&dbusService, &KDBusService::openRequested, &app, [&app](const QList<QUrl> &uris) {
        QApplication::postEvent(&app, new HKDBusOpenEvent(uris));
    });

    {
        auto *const config = HKConfig::self();
        auto *const client = HKHeadsetControl::instance();

        QObject::connect(config, &HKConfig::runInBackgroundChanged, &app, [&app] {
            app.setQuitOnLastWindowClosed(!HKConfig::runInBackground());
        });

        QObject::connect(config, &HKConfig::autoStartChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::startMinimizedChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::useTrayIconChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::runInBackgroundChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::showRefreshTimerChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::primaryDeviceChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::updateRateChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::timeoutChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::testDeviceEnabledChanged, config, &HKConfig::save, Qt::QueuedConnection);
        QObject::connect(config, &HKConfig::testProfileChanged, config, &HKConfig::save, Qt::QueuedConnection);

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

        checkConfigUpdate();

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

    QQmlComponent mainComponent(&engine);
    mainComponent.loadFromModule(HK_APP_ID, "Main"_L1);

    if (mainComponent.createWithInitialProperties({{"visible"_L1, !startMinimized}}, engine.rootContext()) == nullptr) {
        qCCritical(HK_LOGGING, "Failed to load main window!");
        return EXIT_FAILURE;
    }

    return app.exec();
}
