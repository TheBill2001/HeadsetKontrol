#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>

#include <KLocalizedContext>
#include <KLocalizedString>
#include <KAboutData>

#include "about.h"
#include "headsetkontrolconfig.h"
#include "headsetkontrolversion.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("HeadsetKontrol");
    QCoreApplication::setApplicationName(QStringLiteral("HeadsetKontrol"));

    KAboutData aboutData(QStringLiteral("headsetkontrol"),
                         QStringLiteral("HeadsetKontrol"),
                         QStringLiteral(HEADSETKONTROL_VERSION_STRING),
                         i18n("Interface for HeadsetControl (by Sapd) written with Kirigami and KDE Framework"),
                         KAboutLicense::GPL_V3,
                         QStringLiteral("Trần Nam Tuấn (c) 2022"),
                         QString(),
                         QStringLiteral("https://github.com/tuantran1632001/HeadsetKontrol"),
                         QStringLiteral("https://github.com/tuantran1632001/HeadsetKontrol/issues"));
    aboutData.addAuthor(i18n("Trần Nam Tuấn"), i18n("Developer\nMaintainer"), QStringLiteral("tuantran1632001@gmail.com"), QStringLiteral("https://github.com/tuantran1632001"));

    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    // Register qml type
    auto config = HeadsetKontrolConfig::self();
    qmlRegisterSingletonInstance("headsetkontrol", 1, 0, "Config", config);

    qmlRegisterSingletonType<AboutType>("headsetkontrol", 1, 0, "AboutType", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new AboutType();
    });

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/resources/ui/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}

