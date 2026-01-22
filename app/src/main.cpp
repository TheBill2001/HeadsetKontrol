// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkdbusactivationevent.hpp"
#include "hklogging_p.hpp"
#include "hkversion_p.hpp"

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>

#include <KAboutData>
#include <KDBusService>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KirigamiAppDefaults>

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KirigamiAppDefaults::apply(&app);

    KLocalizedString::setApplicationDomain(TRANSLATION_DOMAIN);

    KAboutData aboutData(u"headsetkontrol"_s,
                         u"Headset Kontrol"_s,
                         QStringLiteral(HK_VERSION_STRING),
                         i18nc("@item:intext application description", "Control your headsets with HeadsetControl."),
                         KAboutLicense::GPL_V3,
                         u"Trần Nam Tuấn (c) 2026"_s,
                         {},
                         u"https://gitlab.com/TheBill2001/HeadsetKontrol"_s,
                         u"https://gitlab.com/TheBill2001/HeadsetKontrol/-/issues"_s);

    aboutData.addAuthor(i18nc("name of maintainers", "Trần Nam Tuấn"),
                        i18nc("@info:credit", "Maintainer"),
                        u"tuantran1632001@gmail.com"_s,
                        u"https://gitlab.com/TheBill2001"_s);

    aboutData.setTranslator(i18nc("name of translators", "Trần Nam Tuấn"), u"tuantran1632001@gmail.com"_s);

    aboutData.setOrganizationDomain("thebill2001.com");
    aboutData.setDesktopFileName(QStringLiteral(HK_APP_ID));

    KAboutData::setApplicationData(aboutData);

    {
        QCommandLineParser parser;
        aboutData.setupCommandLine(&parser);
        parser.process(app);
        aboutData.processCommandLine(&parser);
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

    QGuiApplication::setWindowIcon(QIcon::fromTheme(u"com.thebill2001.headsetkontrol"_s));

    QQmlApplicationEngine engine;
    KLocalization::setupLocalizedContext(&engine);
    engine.loadFromModule(HK_APP_ID, u"Main"_s);

    if (engine.rootObjects().isEmpty()) {
        qCFatal(HK_LOGGING).noquote() << i18nc("@info:shell", "Failed to load main window!");
        return EXIT_FAILURE;
    }

    return app.exec();
}
