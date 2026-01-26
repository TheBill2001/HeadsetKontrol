// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkapplication.hpp"

#include "hkconfig.hpp"
#include "hkdbusactivationevent.hpp"
#include "hkheadsetcontrol.hpp"
#include "hklogging.hpp"

#include <QDesktopServices>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTimer>

#include <KAboutData>
#include <KAuthorized>
#include <KLocalizedString>
#include <KNotification>
#include <KStatusNotifierItem>
#include <KWindowSystem>

using namespace Qt::StringLiterals;

HKApplication::HKApplication(QObject *parent)
    : AbstractKirigamiApplication{parent}
    , m_headsetControlActionCollection{new KirigamiActionCollection(this, u"headsetcontrol"_s)}
{
    m_headsetControlActionCollection->setComponentDisplayName(u"HeadsetControl"_s);

    HKApplication::setupActions();

    QCoreApplication::instance()->installEventFilter(this);

    connect(this, &HKApplication::windowChanged, this, &HKApplication::onWindowChanged);

    QObject::connect(HKConfig::self(), &HKConfig::useTrayIconChanged, this, &HKApplication::setupStatusNotifierItem);

    QTimer::singleShot(0, this, &HKApplication::setupStatusNotifierItem);
}

HKApplication::~HKApplication()
{
    HKConfig::self()->save();
}

QAction *HKApplication::startAction()
{
    return action(u"start_headsetcontrol"_s);
}

QAction *HKApplication::stopAction()
{
    return action(u"stop_headsetcontrol"_s);
}

QAction *HKApplication::refreshAction()
{
    return action(u"refresh_headsetcontrol"_s);
}

QAction *HKApplication::quitAction()
{
    return action(KStandardActions::name(KStandardActions::Quit));
}

QAction *HKApplication::commandBarAction()
{
    return action(u"open_kcommand_bar"_s);
}

QAction *HKApplication::aboutAppAction()
{
    return action(u"open_about_page"_s);
}

QAction *HKApplication::keyBindingsAction()
{
    return action(KStandardActions::name(KStandardActions::KeyBindings));
}

QAction *HKApplication::configureAction()
{
    return action(KStandardActions::name(KStandardActions::Preferences));
}

QAction *HKApplication::reportBugAction()
{
    return action(KStandardActions::name(KStandardActions::ReportBug));
}

QBindable<QQuickWindow *> HKApplication::bindableWindow()
{
    return &m_window;
}

QQuickWindow *HKApplication::window() const
{
    return m_window.value();
}

void HKApplication::setWindow(QQuickWindow *window)
{
    m_window = window;
}

QList<KirigamiActionCollection *> HKApplication::actionCollections() const
{
    return AbstractKirigamiApplication::actionCollections() << m_headsetControlActionCollection;
}

bool HKApplication::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == QCoreApplication::instance()) {
        if (event->type() == HKDBusActivateEvent::eventId) {
            if (auto *activateEvent = dynamic_cast<HKDBusActivateEvent *>(event)) {
                activateEvent->accept();

                if (auto *window = m_window.value()) {
                    window->show();
                    KWindowSystem::updateStartupId(window);
                    window->raise();
                    KWindowSystem::activateWindow(window);
                }
            } else {
                Q_UNLIKELY_BRANCH;
                qCDebug(HK_LOGGING, "Invalid HKDBusActivateEvent object.");
            }
            return true;
        }
        return false;
    }
    if (watched == m_window) {
        if (event->type() == QEvent::Close) {
            if (HKConfig::runInBackground() && !HKConfig::useTrayIcon()) {
                auto *notification = new KNotification(u"runInBackgroundWithoutTrayIcon"_s);
                notification->setText(i18nc("notification text", "HeadsetKontrol is running in background without tray icon."));

                auto *action = notification->addAction(i18nc("@action notification", "Enable tray icon"));
                connect(action, &KNotificationAction::activated, HKConfig::self(), [] {
                    HKConfig::setUseTrayIcon(true);
                });

                notification->sendEvent();
            } else if (!HKConfig::runInBackground() && HKConfig::useTrayIcon()) {
                QCoreApplication::quit();
            }
        }
        return false;
    }
    return AbstractKirigamiApplication::eventFilter(watched, event);
}

void HKApplication::setupActions()
{
    AbstractKirigamiApplication::setupActions();

    auto actionName = KStandardActions::name(KStandardActions::ReportBug);
    if (KAuthorized::authorizeAction(actionName)) {
        auto *action = KStandardActions::reportBug(
            this,
            [] {
                QDesktopServices::openUrl(QUrl(KAboutData::applicationData().bugAddress()));
            },
            mainCollection());
        mainCollection()->addAction(action->objectName(), action);
    }

    actionName = "start_headsetcontrol"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto *action = m_headsetControlActionCollection->addAction(actionName, HKHeadsetControl::instance(), &HKHeadsetControl::start);
        action->setText(i18nc("@action start update headsetcontrol", "&Start"));
        action->setIcon(QIcon::fromTheme("media-playback-start"_L1));
        action->setDisabled(HKHeadsetControl::isRunning());

        m_headsetControlActionCollection->addAction(action->objectName(), action);
        m_headsetControlActionCollection->setDefaultShortcut(action, Qt::CTRL | Qt::Key_R);

        connect(HKHeadsetControl::instance(), &HKHeadsetControl::runningChanged, action, &QAction::setDisabled);
    }

    actionName = "stop_headsetcontrol"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto *action = m_headsetControlActionCollection->addAction(actionName, HKHeadsetControl::instance(), &HKHeadsetControl::stop);
        action->setText(i18nc("@action stop update headsetcontrol", "Sto&p"));
        action->setIcon(QIcon::fromTheme("media-playback-stop"_L1));
        action->setEnabled(HKHeadsetControl::isRunning());

        m_headsetControlActionCollection->addAction(action->objectName(), action);
        m_headsetControlActionCollection->setDefaultShortcut(action, Qt::CTRL | Qt::Key_P);

        connect(HKHeadsetControl::instance(), &HKHeadsetControl::runningChanged, action, &QAction::setEnabled);
    }

    actionName = "refresh_headsetcontrol"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto *action = m_headsetControlActionCollection->addAction(actionName, HKHeadsetControl::instance(), qOverload<>(&HKHeadsetControl::refresh));
        action->setText(i18nc("@action refresh headsetcontrol", "&Refresh"));
        action->setIcon(QIcon::fromTheme("view-refresh"_L1));

        m_headsetControlActionCollection->addAction(action->objectName(), action);
        m_headsetControlActionCollection->setDefaultShortcut(action, Qt::CTRL | Qt::Key_F5);
    }

    readSettings();
}

void HKApplication::onWindowChanged()
{
    if (m_previousWindow != nullptr) {
        m_previousWindow->removeEventFilter(this);
    }
    auto *const window = m_window.value();
    if (window != nullptr) {
        window->installEventFilter(this);
    }
    if (m_statusNotifierItem != nullptr) {
        m_statusNotifierItem->setAssociatedWindow(window);
    }
    m_previousWindow = window;
}

void HKApplication::setupStatusNotifierItem()
{
    delete m_statusNotifierItem;
    m_statusNotifierItem = nullptr;
    if (HKConfig::useTrayIcon()) {
        m_statusNotifierItem = new KStatusNotifierItem(this);
        m_statusNotifierItem->setAssociatedWindow(window());
        m_statusNotifierItem->setStandardActionsEnabled(false);
        m_statusNotifierItem->setStatus(KStatusNotifierItem::Active);
        m_statusNotifierItem->setIconByName(u"headsetkontrol"_s);

        auto *const menu = m_statusNotifierItem->contextMenu();
        menu->addSection(i18nc("@title:menu", "Help"));
        menu->addAction(aboutAppAction());
        menu->addAction(reportBugAction());

        menu->addSection(i18nc("@title:menu", "Control"));
        menu->addAction(startAction());
        menu->addAction(stopAction());
        menu->addAction(refreshAction());

        menu->addSection(i18nc("@title:menu", "Settings"));
        menu->addAction(keyBindingsAction());
        menu->addAction(configureAction());

        menu->addSeparator();
        menu->addAction(quitAction());
    }
}
