#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>
#include <QTimer>

#include <KAboutData>
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>

#include "headsetkontrolapplication.h"
#include "headsetkontrolnotifieritem.h"

HeadsetKontrolNotifierItem::HeadsetKontrolNotifierItem(HeadsetKontrolApplication *parent)
    : KStatusNotifierItem{parent}
    , m_parent{parent}
{
    // QMenu *menu = new QMenu();

    // auto viewAction = new QAction(i18nc("@action:inmenu", "Show"), parent);
    // connect(viewAction, &QAction::triggered, this, &HeadsetKontrolNotifierItem::showWindow);
    // connect(this, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
    //     if (reason == QSystemTrayIcon::Trigger) {
    //         Q_EMIT showWindow();
    //     }
    // });
    // menu->addAction(viewAction);

    // auto settingAction = new QAction(i18nc("@action:inmenu", "Settings"), parent);
    // connect(settingAction, &QAction::triggered, this, &HeadsetKontrolNotifierItem::showSettings);
    // menu->addAction(settingAction);

    // menu->addSeparator();

    // auto quitAction = new QAction(i18nc("@action:inmenu", "Quit"), parent);
    // quitAction->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
    // connect(quitAction, &QAction::triggered, QCoreApplication::instance(), QCoreApplication::quit);

    // menu->addAction(quitAction);

    // setContextMenu(menu);

    // setIcon(QIcon::fromTheme(QStringLiteral("headsetkontrol")));

    // show();

    m_menu = new QMenu();

    setStandardActionsEnabled(false);
    setIconByName(QStringLiteral("headsetkontrol"));
    setStatus(KStatusNotifierItem::Active);
    setContextMenu(m_menu);

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Help"));
    m_menu->addAction(m_parent->actionCollection()->action(KStandardAction::name(KStandardAction::AboutApp)));
    m_menu->addAction(m_parent->actionCollection()->action(KStandardAction::name(KStandardAction::ReportBug)));

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Settings"));
    m_menu->addAction(m_parent->actionCollection()->action(KStandardAction::name(KStandardAction::Preferences)));

    m_menu->addSeparator();
    m_menu->addAction(m_parent->actionCollection()->action(KStandardAction::name(KStandardAction::Quit)));
}

HeadsetKontrolNotifierItem::~HeadsetKontrolNotifierItem()
{
}
