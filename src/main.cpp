#include <KLocalizedTranslator>
#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QScopedPointer>
#include <QUrl>

#include <KAboutData>
#include <KDBusService>
#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "headsetkontrolversion.h"

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    qSetMessagePattern(QStringLiteral("[%{time yyyy-MM-dd h:mm:ss}] [%{type}] [%{file}:%{line}] %{message}"));
#else
    qSetMessagePattern(QStringLiteral("[%{time yyyy-MM-dd h:mm:ss}] [%{type}] %{message}"));
#endif

    KIconTheme::initTheme();

    QApplication app(argc, argv);

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(u"org.kde.desktop"_s);
    }

    KLocalizedString::setApplicationDomain("com.gitlab.thebill2001.headsetkontrol");

    KAboutData aboutData(u"headsetkontrol"_s,
                         u"Headset Kontrol"_s,
                         QStringLiteral(HEADSETKONTROL_VERSION_STRING),
                         i18n("Interface for HeadsetControl (by Sapd) written with Kirigami and KDE Framework"),
                         KAboutLicense::GPL_V3,
                         u"Trần Nam Tuấn (c) 2024"_s,
                         QString(),
                         "https://gitlab.com/TheBill2001/HeadsetKontrol"_L1,
                         "https://gitlab.com/TheBill2001/HeadsetKontrol/-/issues"_L1);
    aboutData.addAuthor(i18n("Trần Nam Tuấn"),
                        i18nc("@info:credit", "Developer, Maintainer"),
                        "tuantran1632001@gmail.com"_L1,
                        "https://gitlab.com/TheBill2001"_L1);
    aboutData.setDesktopFileName("headsetkontrol"_L1);
    aboutData.setOrganizationDomain("thebill2001.gitlab.com");

    KAboutData::setApplicationData(aboutData);

    QGuiApplication::setWindowIcon(QIcon::fromTheme(u"headsetkontrol"_s));

    KDBusService service(KDBusService::Unique);

    // Translation
    QCoreApplication::installTranslator(new KLocalizedTranslator(&app));

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [&service](QObject *object) {
        if (object && object->objectName() == "root"_L1) {
            QObject::connect(&service, &KDBusService::activateRequested, object, [object]() {
                QMetaObject::invokeMethod(object, "restore", Qt::QueuedConnection);
            });
        }
    });

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("com.gitlab.thebill2001.headsetkontrol", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
