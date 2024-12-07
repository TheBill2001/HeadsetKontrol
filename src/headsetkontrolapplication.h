#ifndef HEADSETKONTROLAPPLICATION_H
#define HEADSETKONTROLAPPLICATION_H

#include "headsetkontrolnotifieritem.h"

#include <QAction>
#include <QQmlEngine>

#include <AbstractKirigamiApplication>
#include <KStandardActions>

class HeadsetControlDevice;
class HeadsetKontrolApplication : public AbstractKirigamiApplication
{
    Q_OBJECT
    QML_ELEMENT
    QML_NAMED_ELEMENT(App)
public:
    enum AppAction {
        AboutApp,
        KCommandBar,
        KeyBindings = KStandardActions::KeyBindings,
        Preferences = KStandardActions::Preferences,
        ReportBug = KStandardActions::ReportBug,
        ConfigureNotifications = KStandardActions::ConfigureNotifications,
        Quit = KStandardActions::Quit,

        // HeadsetControl actions
        HeadsetControlStop,
        HeadsetControlStart,
        HeadsetControlRefresh
    };
    Q_ENUM(AppAction)

    explicit HeadsetKontrolApplication(QObject *parent = nullptr);
    ~HeadsetKontrolApplication();

    Q_INVOKABLE QAction *action(AppAction id);

    QList<KirigamiActionCollection *> actionCollections() const override;

Q_SIGNALS:
    void showWindow();
    void showDevice(HeadsetControlDevice *device);

protected:
    void setupActions() override;

private:
    HeadsetKontrolNotifierItem *m_notifierItem;
    KirigamiActionCollection *m_headsetcontrolActionCollection;

private Q_SLOTS:
    void updateNotifierItem();
};

#endif // HEADSETKONTROLAPPLICATION_H
