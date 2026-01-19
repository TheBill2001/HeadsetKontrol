/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
*/

#include <KirigamiApp>
#include <QtGlobal>

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#else
#include <QApplication>
#endif
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>

#include "version-headsetkontrol.h"
#include <KAboutData>
#include <KLocalizedString>

#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

#ifdef Q_OS_ANDROID
Q_DECL_EXPORT
#endif
int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    KirigamiAppDefaults::apply(&app);

    KLocalizedString::setApplicationDomain("headsetkontrol");
    QCoreApplication::setOrganizationName(u"KDE"_s);

    KAboutData aboutData(
        // The program name used internally.
        u"headsetkontrol"_s,
        // A displayable program name string.
        i18nc("@title", "HeadsetKontrol"),
        // The program version string.
        QStringLiteral(HEADSETKONTROL_VERSION_STRING),
        // Short description of what the app does.
        i18n("Application Description"),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("(c) 2026"));
    aboutData.addAuthor(i18nc("@info:credit", "Trần Nam Tuấn"),
                        i18nc("@info:credit", "Maintainer"),
                        u"tuantran1632001@gmail.com"_s,
                        u"https://yourwebsite.com"_s);
    aboutData.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    KAboutData::setApplicationData(aboutData);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(u"org.kde.headsetkontrol"_s));

    QQmlApplicationEngine engine;

    KLocalization::setupLocalizedContext(&engine);

    auto config = HeadsetKontrolConfig::self();

    qmlRegisterSingletonInstance("org.kde.headsetkontrol.private", 1, 0, "Config", config);

    {
        QCommandLineParser parser;
        aboutData.setupCommandLine(&parser);
        parser.process(app);
        aboutData.processCommandLine(&parser);
    }

    if (!engine.loadFromModule("org.kde.headsetkontrol", u"Main"_s)) {
        return -1;
    }

    return app.exec();
}
