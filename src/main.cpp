#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>

#include <KLocalizedContext>
#include <KLocalizedString>
#include <KAboutData>

#include "config-headsetkontrol.h"
#include "about.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("HeadsetKontrol");
    QCoreApplication::setApplicationName(QStringLiteral("HeadsetKontrol"));

    KAboutData aboutData(QStringLiteral("headsetkontrol"),
                         i18nc("@title", "HeadsetKontrol"),
                         QStringLiteral(HEADSETKONTROL_VERSION_STRING),
                         i18n("Interface for HeadsetControl (by Sapd) written with Kirigami and KDE Framework"),
                         KAboutLicense::GPL_V3,
                         i18n("(c) 2022"));
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

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

