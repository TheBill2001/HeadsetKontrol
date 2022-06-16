#include <QApplication>
#include <QQmlApplicationEngine>
#include <QScopedPointer>
#include <QUrl>
#include <QtQml>

#include <KLocalizedContext>
#include <KLocalizedString>

#include "about.h"
#include "appcontroller.h"
#include "headsetkontrolversion.h"

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    qSetMessagePattern(QStringLiteral("[%{time yyyy-MM-dd h:mm:ss}] [%{type}] [%{file}:%{line}] %{message}"));
#else
    qSetMessagePattern(QStringLiteral("[%{time yyyy-MM-dd h:mm:ss}] [%{type}] %{message}"));
#endif

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(QStringLiteral(":/icons/hicolor/scalable/apps/headsetkontrol.svg")));

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
    aboutData.addAuthor(i18n("Trần Nam Tuấn"),
                        i18n("Developer\nMaintainer"),
                        QStringLiteral("tuantran1632001@gmail.com"),
                        QStringLiteral("https://github.com/tuantran1632001"));

    KAboutData::setApplicationData(aboutData);

    QScopedPointer appControllerPointer(new AppController(&app));

    QQmlApplicationEngine engine;

    // Register qml type
    qmlRegisterUncreatableType<HeadsetControl>("headsetkontrol", 1, 0, "HeadsetControl", QStringLiteral("SINGLETON"));
    qmlRegisterUncreatableType<HeadsetKontrolConfig>("headsetkontrol", 1, 0, "Config", QStringLiteral("Type is singleton"));

    qmlRegisterSingletonInstance("headsetkontrol", 1, 0, "AppController", appControllerPointer.get());

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:/resources/ui/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
