#include <KLocalizedTranslator>
#include <QApplication>
#include <QCommandLineParser>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QScopedPointer>
#include <QUrl>

#include <KAboutData>
#include <KDBusAddons/KDBusService>
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

    auto message = QDBusMessage::createMethodCall(QStringLiteral("com.gitlab.thebill2001.headsetkontrol"),
                                                  QStringLiteral("/HeadsetKontrol"),
                                                  QString(),
                                                  QStringLiteral("pid"));
    auto reply = QDBusConnection::sessionBus().call(message);

    if (reply.type() == QDBusMessage::ReplyMessage) {
        auto args = reply.arguments();
        auto val = args.at(0).toLongLong();
        qInfo() << "Found running instance:" << val;

        auto message = QDBusMessage::createMethodCall(QStringLiteral("com.gitlab.thebill2001.headsetkontrol"),
                                                      QStringLiteral("/HeadsetKontrol"),
                                                      QString(),
                                                      QStringLiteral("restore"));
        auto reply = QDBusConnection::sessionBus().call(message);

        if (reply.type() == QDBusMessage::ReplyMessage)
            qInfo() << "Restore instance:" << val;
        else
            qFatal("Cannot restore instance: %lld\n", val);

        return 0;
    }

    KIconTheme::initTheme();

    QApplication app(argc, argv);

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(u"org.kde.desktop"_s);
    }

    KLocalizedString::setApplicationDomain("com.gitlab.thebill2001.headsetkontrol");

    qInfo().noquote() << u"Start new instance. PID: %1"_s.arg(QString::number(app.applicationPid()));

    KAboutData aboutData(u"headsetkontrol"_s,
                         u"Headset Kontrol"_s,
                         QStringLiteral(HEADSETKONTROL_VERSION_STRING),
                         i18n("Interface for HeadsetControl (by Sapd) written with Kirigami and KDE Framework"),
                         KAboutLicense::GPL_V3,
                         QStringLiteral("Trần Nam Tuấn (c) 2024"),
                         QString(),
                         QStringLiteral("https://gitlab.com/TheBill2001/HeadsetKontrol"),
                         QStringLiteral("https://gitlab.com/TheBill2001/HeadsetKontrol/-/issues"));
    aboutData.addAuthor(i18n("Trần Nam Tuấn"),
                        i18nc("@info:credit", "Developer, Maintainer"),
                        QStringLiteral("tuantran1632001@gmail.com"),
                        QStringLiteral("https://gitlab.com/TheBill2001"));
    aboutData.setDesktopFileName(u"headsetkontrol"_s);
    aboutData.setOrganizationDomain("thebill2001.gitlab.com");

    KAboutData::setApplicationData(aboutData);

    QGuiApplication::setWindowIcon(QIcon::fromTheme(u"headsetkontrol"_s));

    KDBusService service(KDBusService::Unique);

    // Translation
    QCoreApplication::installTranslator(new KLocalizedTranslator(&app));

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule("com.gitlab.thebill2001.headsetkontrol", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
