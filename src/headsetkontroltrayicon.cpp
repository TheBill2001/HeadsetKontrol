#include <QCoreApplication>
#include <QDebug>
#include <QMenu>

#include <KLocalizedString>

#include "headsetkontrolapplication.h"
#include "headsetkontroltrayicon.h"

HeadsetKontrolTrayIcon::HeadsetKontrolTrayIcon(HeadsetKontrolApplication *parent)
    : QSystemTrayIcon{parent}
{
    QMenu *menu = new QMenu();

    auto viewAction = new QAction(i18nc("@action:inmenu", "Show"), parent);
    connect(viewAction, &QAction::triggered, this, &HeadsetKontrolTrayIcon::showWindow);
    connect(this, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            Q_EMIT showWindow();
        }
    });
    menu->addAction(viewAction);

    auto settingAction = new QAction(i18nc("@action:inmenu", "Settings"), parent);
    connect(settingAction, &QAction::triggered, this, &HeadsetKontrolTrayIcon::showSettings);
    menu->addAction(settingAction);

    menu->addSeparator();

    auto quitAction = new QAction(i18nc("@action:inmenu", "Quit"), parent);
    quitAction->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
    connect(quitAction, &QAction::triggered, QCoreApplication::instance(), QCoreApplication::quit);

    menu->addAction(quitAction);

    setContextMenu(menu);

    setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol")));

    show();
}
