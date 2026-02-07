// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkapplication.hpp"

#include "hkconfig.hpp"
#include "hkheadsetcontrol.hpp"

#include <QDesktopServices>

#include <KAboutData>
#include <KAuthorized>
#include <KLocalizedString>

using namespace Qt::StringLiterals;

HKApplication::HKApplication(QObject *parent)
    : AbstractKirigamiApplication{parent}
    , m_headsetControlActionCollection{new KirigamiActionCollection(this, u"headsetcontrol"_s)}
{
    m_headsetControlActionCollection->setComponentDisplayName(u"HeadsetControl"_s);

    HKApplication::setupActions();
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

QList<KirigamiActionCollection *> HKApplication::actionCollections() const
{
    return AbstractKirigamiApplication::actionCollections() << m_headsetControlActionCollection;
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
