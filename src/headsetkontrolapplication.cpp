#include <QDBusConnection>
#include <QDesktopServices>

#include <KAuthorized>
#include <KLocalizedString>
#include <KNotifyConfigWidget>

#include "headsetcontrol.h"
#include "headsetkontrolapplication.h"
#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

HeadsetKontrolApplication::HeadsetKontrolApplication(QObject *parent)
    : AbstractKirigamiApplication{parent}
    , m_notifierItem{nullptr}
    , m_headsetcontrolActionCollection{new KirigamiActionCollection(this, "Heaset Control"_L1)}
{
    m_headsetcontrolActionCollection->setComponentDisplayName("Heaset Control"_L1);

    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::RunInBackgroundChanged, this, &HeadsetKontrolApplication::updateNotifierItem);

    setupActions();

    connect(this->mainCollection(), &KirigamiActionCollection::changed, this, &HeadsetKontrolApplication::updateNotifierItem);
}

HeadsetKontrolApplication::~HeadsetKontrolApplication()
{
}

QAction *HeadsetKontrolApplication::action(AppAction id)
{
    QString actionName;
    switch (id) {
    case KCommandBar:
        actionName = "open_kcommand_bar"_L1;
        break;
    case AboutApp:
        actionName = "open_about_page"_L1;
        break;
    case HeadsetControlStop:
        actionName = "stop_headsetcontrol"_L1;
        break;
    case HeadsetControlStart:
        actionName = "start_headsetcontrol"_L1;
        break;
    case HeadsetControlRefresh:
        actionName = "refresh_headsetcontrol"_L1;
        break;
    default:
        actionName = KStandardActions::name(static_cast<KStandardActions::StandardAction>(id));
        break;
    }

    return AbstractKirigamiApplication::action(actionName);
}

QList<KirigamiActionCollection *> HeadsetKontrolApplication::actionCollections() const
{
    auto list = AbstractKirigamiApplication::actionCollections();
    list << m_headsetcontrolActionCollection;
    return list;
}

void HeadsetKontrolApplication::setupActions()
{
    AbstractKirigamiApplication::setupActions();

    auto actionName = KStandardActions::name(KStandardActions::ReportBug);
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = KStandardActions::reportBug(
            this,
            []() {
                QDesktopServices::openUrl(QUrl(KAboutData::applicationData().bugAddress()));
            },
            mainCollection());

        mainCollection()->addAction(action->objectName(), action);
    }

    actionName = KStandardActions::name(KStandardActions::ReportBug);
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = KStandardActions::configureNotifications(
            this,
            []() {
                KNotifyConfigWidget::configure();
            },
            mainCollection());
        mainCollection()->addAction(action->objectName(), action);
    }

    actionName = "stop_headsetcontrol"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = m_headsetcontrolActionCollection->addAction(actionName, this, []() {
            HeadsetControl::instance()->stop();
        });
        action->setText(i18nc("@action stop update headsetcontrol", "Stop"));
        action->setIcon(QIcon::fromTheme(u"media-playback-stop"_s));
        action->setDisabled(!HeadsetControl::instance()->isRunning());
        action->setVisible(HeadsetControl::instance()->isRunning());

        m_headsetcontrolActionCollection->addAction(action->objectName(), action);
        m_headsetcontrolActionCollection->setDefaultShortcuts(action,
                                                              {QKeySequence(Qt::CTRL | Qt::Key_MediaPause),
                                                               QKeySequence(Qt::CTRL | Qt::Key_MediaStop),
                                                               QKeySequence(Qt::CTRL | Qt::Key_MediaTogglePlayPause),
                                                               QKeySequence(Qt::CTRL | Qt::Key_Pause)});

        connect(HeadsetControl::instance(), &HeadsetControl::isRunningChanged, action, [action]() {
            action->setDisabled(!HeadsetControl::instance()->isRunning());
            action->setVisible(HeadsetControl::instance()->isRunning());
        });
    }

    actionName = "start_headsetcontrol"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = m_headsetcontrolActionCollection->addAction(actionName, this, []() {
            HeadsetControl::instance()->start();
        });
        action->setText(i18nc("@action start update headsetcontrol", "Start"));
        action->setIcon(QIcon::fromTheme(u"media-playback-start"_s));
        action->setDisabled(HeadsetControl::instance()->isRunning());
        action->setVisible(!HeadsetControl::instance()->isRunning());

        m_headsetcontrolActionCollection->addAction(action->objectName(), action);
        m_headsetcontrolActionCollection->setDefaultShortcuts(
            action,
            {QKeySequence(Qt::CTRL | Qt::Key_MediaPlay), QKeySequence(Qt::CTRL | Qt::Key_MediaTogglePlayPause)});

        connect(HeadsetControl::instance(), &HeadsetControl::isRunningChanged, action, [action]() {
            action->setDisabled(HeadsetControl::instance()->isRunning());
            action->setVisible(!HeadsetControl::instance()->isRunning());
        });
    }

    actionName = "refresh_headsetcontrol"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = m_headsetcontrolActionCollection->addAction(actionName, this, []() {
            HeadsetControl::instance()->refresh();
        });
        action->setText(i18nc("@action refresh headsetcontrol", "Refresh"));
        action->setIcon(QIcon::fromTheme(u"view-refresh"_s));

        m_headsetcontrolActionCollection->addAction(action->objectName(), action);
        m_headsetcontrolActionCollection->setDefaultShortcuts(action, {QKeySequence(Qt::CTRL | Qt::Key_Reload), QKeySequence(Qt::CTRL | Qt::Key_F5)});
    }

    readSettings();
}

void HeadsetKontrolApplication::updateNotifierItem()
{
    if (!HeadsetKontrolConfig::instance()->runInBackground() && m_notifierItem != nullptr) {
        delete m_notifierItem;
        m_notifierItem = nullptr;
    } else if (HeadsetKontrolConfig::instance()->runInBackground()) {
        if (m_notifierItem != nullptr) {
            delete m_notifierItem;
            m_notifierItem = nullptr;
        }
        m_notifierItem = new HeadsetKontrolNotifierItem(this);
    }
}