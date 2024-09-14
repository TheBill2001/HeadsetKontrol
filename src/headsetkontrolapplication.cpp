#include <QDBusConnection>
#include <QDesktopServices>

#include <KActionCollection>
#include <KLocalizedString>
#include <KNotifyConfigWidget>
#include <KShortcutsDialog>
#include <KStandardAction>

#include "headsetcontrol.h"
#include "headsetkontrolapplication.h"
#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

HeadsetKontrolApplication::HeadsetKontrolApplication(QObject *parent)
    : QObject{parent}
    , m_notifierItem{nullptr}
{
    setObjectName(u"HeadsetKontrol"_s);

    QDBusConnection::sessionBus().registerService(u"com.gitlab.thebill2001.headsetkontrol"_s);
    QDBusConnection::sessionBus().registerObject(u"/HeadsetKontrol"_s, this, QDBusConnection::QDBusConnection::ExportScriptableInvokables);

    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::RunInBackgroundChanged, this, &HeadsetKontrolApplication::onRunInBackgroundChanged);

    initAction();

    onRunInBackgroundChanged();
}

HeadsetKontrolApplication::~HeadsetKontrolApplication()
{
}

qint64 HeadsetKontrolApplication::pid() const
{
    return QCoreApplication::applicationPid();
}

void HeadsetKontrolApplication::restore()
{
    Q_EMIT showWindow();
}

KActionCollection *HeadsetKontrolApplication::actionCollection() const
{
    return m_actionCollection;
}

QAction *HeadsetKontrolApplication::quitAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::Quit));
}

QAction *HeadsetKontrolApplication::configureAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::Preferences));
}

QAction *HeadsetKontrolApplication::configureNotificationsAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::ConfigureNotifications));
}

QAction *HeadsetKontrolApplication::configureKeyBindingsAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::KeyBindings));
}

QAction *HeadsetKontrolApplication::aboutAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::AboutApp));
}

QAction *HeadsetKontrolApplication::reportBugAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::ReportBug));
}

QAction *HeadsetKontrolApplication::startHeadsetControlAction() const
{
    return actionCollection()->action(u"start-headsetcontrol"_s);
}

QAction *HeadsetKontrolApplication::stopHeadsetControlAction() const
{
    return actionCollection()->action(u"stop-headsetcontrol"_s);
}

QAction *HeadsetKontrolApplication::refreshHeadsetControlAction() const
{
    return actionCollection()->action(u"refresh-headsetcontrol"_s);
}

void HeadsetKontrolApplication::quit()
{
    QCoreApplication::quit();
}

void HeadsetKontrolApplication::onRunInBackgroundChanged()
{
    if (!HeadsetKontrolConfig::instance()->runInBackground() && m_notifierItem != nullptr) {
        delete m_notifierItem;
        m_notifierItem = nullptr;
    } else if (HeadsetKontrolConfig::instance()->runInBackground() && m_notifierItem == nullptr) {
        m_notifierItem = new HeadsetKontrolNotifierItem(this);
    }
}

void HeadsetKontrolApplication::initAction()
{
    m_actionCollection = new KActionCollection(this);

    QAction *action = KStandardAction::quit(this, &HeadsetKontrolApplication::quit, m_actionCollection);
    m_actionCollection->setDefaultShortcut(action, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Q));

    KStandardAction::aboutApp(this, &HeadsetKontrolApplication::showAbout, m_actionCollection);

    KStandardAction::reportBug(
        this,
        []() {
            QDesktopServices::openUrl(QUrl(KAboutData::applicationData().bugAddress()));
        },
        m_actionCollection);

    KStandardAction::preferences(this, &HeadsetKontrolApplication::showSettings, m_actionCollection);

    KStandardAction::keyBindings(
        this,
        [this]() {
            KShortcutsDialog dialog(KShortcutsEditor::AllActions, KShortcutsEditor::LetterShortcutsAllowed);
            dialog.addCollection(actionCollection());
            dialog.configure();
        },
        m_actionCollection);

    KStandardAction::configureNotifications(
        this,
        []() {
            KNotifyConfigWidget::configure();
        },
        m_actionCollection);

    action = m_actionCollection->addAction(u"stop-headsetcontrol"_s);
    action->setText(i18nc("@action stop update headsetcontrol", "Stop"));
    action->setIcon(QIcon::fromTheme(u"media-playback-stop"_s));
    action->setDisabled(!HeadsetControl::instance()->isRunning());
    action->setVisible(HeadsetControl::instance()->isRunning());
    m_actionCollection->setDefaultShortcuts(action,
                                            {QKeySequence(Qt::CTRL | Qt::Key_MediaPause),
                                             QKeySequence(Qt::CTRL | Qt::Key_MediaStop),
                                             QKeySequence(Qt::CTRL | Qt::Key_MediaTogglePlayPause),
                                             QKeySequence(Qt::CTRL | Qt::Key_Pause)});
    connect(action, &QAction::triggered, this, []() {
        HeadsetControl::instance()->stop();
    });
    connect(HeadsetControl::instance(), &HeadsetControl::isRunningChanged, action, [action]() {
        action->setDisabled(!HeadsetControl::instance()->isRunning());
        action->setVisible(HeadsetControl::instance()->isRunning());
    });

    action = m_actionCollection->addAction(u"start-headsetcontrol"_s);
    action->setText(i18nc("@action start update headsetcontrol", "Start"));
    action->setIcon(QIcon::fromTheme(u"media-playback-start"_s));
    action->setDisabled(HeadsetControl::instance()->isRunning());
    action->setVisible(!HeadsetControl::instance()->isRunning());
    m_actionCollection->setDefaultShortcuts(action, {QKeySequence(Qt::CTRL | Qt::Key_MediaPlay), QKeySequence(Qt::CTRL | Qt::Key_MediaTogglePlayPause)});
    connect(action, &QAction::triggered, this, []() {
        HeadsetControl::instance()->start();
    });
    connect(HeadsetControl::instance(), &HeadsetControl::isRunningChanged, action, [action]() {
        action->setDisabled(HeadsetControl::instance()->isRunning());
        action->setVisible(!HeadsetControl::instance()->isRunning());
    });

    action = m_actionCollection->addAction(u"refresh-headsetcontrol"_s);
    action->setText(i18nc("@action refresh headsetcontrol", "Refresh"));
    action->setIcon(QIcon::fromTheme(u"view-refresh"_s));
    m_actionCollection->setDefaultShortcuts(action, {QKeySequence(Qt::CTRL | Qt::Key_Reload), QKeySequence(Qt::CTRL | Qt::Key_F5)});
    connect(action, &QAction::triggered, this, []() {
        HeadsetControl::instance()->refresh();
    });
}
