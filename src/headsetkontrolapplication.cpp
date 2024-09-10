#include <QDBusConnection>
#include <QDesktopServices>

#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>

#include "headsetkontrolapplication.h"
#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

HeadsetKontrolApplication::HeadsetKontrolApplication(QObject *parent)
    : QObject{parent}
    , m_notifierItem{nullptr}
{
    setObjectName(u"HeadsetKontrol"_s);

    QDBusConnection::sessionBus().registerService(QStringLiteral("com.gitlab.thebill2001.headsetkontrol"));
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/HeadsetKontrol"), this, QDBusConnection::ExportScriptableContents);

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

QAction *HeadsetKontrolApplication::aboutAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::AboutApp));
}

QAction *HeadsetKontrolApplication::reportBugAction() const
{
    return actionCollection()->action(KStandardAction::name(KStandardAction::ReportBug));
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
    action = KStandardAction::aboutApp(this, &HeadsetKontrolApplication::showAbout, m_actionCollection);
    action = KStandardAction::reportBug(
        this,
        []() {
            QDesktopServices::openUrl(QUrl(KAboutData::applicationData().bugAddress()));
        },
        m_actionCollection);
    action = KStandardAction::preferences(this, &HeadsetKontrolApplication::showSettings, m_actionCollection);
}
