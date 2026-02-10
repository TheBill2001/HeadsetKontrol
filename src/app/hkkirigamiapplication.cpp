// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkkirigamiapplication.hpp"

#include "hkheadsetcontrol.hpp"

#include <QDesktopServices>

#include <KAboutData>
#include <KAuthorized>
#include <KLocalizedString>

using namespace Qt::StringLiterals;

HKKirigamiApplication::HKKirigamiApplication(QObject *parent)
    : AbstractKirigamiApplication{parent}
    , m_headsetControlActionCollection{new KirigamiActionCollection(this, u"headsetcontrol"_s)}
{
    m_headsetControlActionCollection->setComponentDisplayName(i18nc("HeadsetControl name", "HeadsetControl"));

    HKKirigamiApplication::setupActions();
}

QAction *HKKirigamiApplication::startAction()
{
    return action(u"start_headsetcontrol"_s);
}

QAction *HKKirigamiApplication::stopAction()
{
    return action(u"stop_headsetcontrol"_s);
}

QAction *HKKirigamiApplication::refreshAction()
{
    return action(u"refresh_headsetcontrol"_s);
}

QAction *HKKirigamiApplication::quitAction()
{
    return action(KStandardActions::name(KStandardActions::Quit));
}

QAction *HKKirigamiApplication::commandBarAction()
{
    return action(u"open_kcommand_bar"_s);
}

QAction *HKKirigamiApplication::aboutAppAction()
{
    return action(u"open_about_page"_s);
}

QAction *HKKirigamiApplication::keyBindingsAction()
{
    return action(KStandardActions::name(KStandardActions::KeyBindings));
}

QAction *HKKirigamiApplication::configureAction()
{
    return action(KStandardActions::name(KStandardActions::Preferences));
}

QAction *HKKirigamiApplication::reportBugAction()
{
    return action(KStandardActions::name(KStandardActions::ReportBug));
}

QList<KirigamiActionCollection *> HKKirigamiApplication::actionCollections() const
{
    return AbstractKirigamiApplication::actionCollections() << m_headsetControlActionCollection;
}

void HKKirigamiApplication::setupActions()
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
