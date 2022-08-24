#include <QCoreApplication>
#include <QDebug>
#include <QMenu>

#include <KLocalizedString>

#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon{parent}
{
    QMenu *menu = new QMenu();

    auto viewAction = new QAction(i18n("Show"), parent);
    connect(viewAction, &QAction::triggered, this, &TrayIcon::showWindow);
    connect(this, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            Q_EMIT showWindow();
        }
    });
    menu->addAction(viewAction);

    auto settingAction = new QAction(i18n("Settings"), parent);
    connect(settingAction, &QAction::triggered, this, &TrayIcon::showSettings);
    menu->addAction(settingAction);

    menu->addSeparator();

    auto quitAction = new QAction(i18n("Quit"), parent);
    quitAction->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
    connect(quitAction, &QAction::triggered, QCoreApplication::instance(), QCoreApplication::quit);

    menu->addAction(quitAction);

    setContextMenu(menu);
}
