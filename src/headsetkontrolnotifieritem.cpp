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
    m_menu = new QMenu();

    setStandardActionsEnabled(false);
    setIconByName(QStringLiteral("headsetkontrol"));
    setStatus(KStatusNotifierItem::Active);
    setContextMenu(m_menu);

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Help"));
    m_menu->addAction(m_parent->aboutAction());
    m_menu->addAction(m_parent->reportBugAction());

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Actions"));
    m_menu->addAction(m_parent->startHeadsetControlAction());
    m_menu->addAction(m_parent->stopHeadsetControlAction());
    m_menu->addAction(m_parent->refreshHeadsetControlAction());

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Settings"));
    m_menu->addAction(m_parent->configureKeyBindingsAction());
    m_menu->addAction(m_parent->configureAction());
    m_menu->addAction(m_parent->configureNotificationsAction());

    m_menu->addSeparator();
    m_menu->addAction(m_parent->quitAction());

    connect(this, &HeadsetKontrolNotifierItem::activateRequested, parent, &HeadsetKontrolApplication::showWindow);
}

HeadsetKontrolNotifierItem::~HeadsetKontrolNotifierItem()
{
}
