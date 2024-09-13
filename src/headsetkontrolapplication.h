#ifndef HEADSETKONTROLAPPLICATION_H
#define HEADSETKONTROLAPPLICATION_H

#include <QAction>
#include <QQmlEngine>

#include "headsetkontrolnotifieritem.h"

class HeadsetKontrolApplication : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(App)
    QML_SINGLETON

    Q_PROPERTY(QAction *quitAction READ quitAction CONSTANT FINAL)
    Q_PROPERTY(QAction *configureAction READ configureAction CONSTANT FINAL)
    Q_PROPERTY(QAction *configureNotificationsAction READ configureNotificationsAction CONSTANT FINAL)
    Q_PROPERTY(QAction *aboutAction READ aboutAction CONSTANT FINAL)
    Q_PROPERTY(QAction *configureKeyBindingsAction READ configureKeyBindingsAction CONSTANT FINAL)
    Q_PROPERTY(QAction *reportBugAction READ reportBugAction CONSTANT FINAL)
    Q_PROPERTY(QAction *startHeadsetControlAction READ startHeadsetControlAction CONSTANT FINAL)
    Q_PROPERTY(QAction *stopHeadsetControlAction READ stopHeadsetControlAction CONSTANT FINAL)
    Q_PROPERTY(QAction *refreshHeadsetControlAction READ refreshHeadsetControlAction CONSTANT FINAL)
public:
    explicit HeadsetKontrolApplication(QObject *parent = nullptr);
    ~HeadsetKontrolApplication();

    Q_SCRIPTABLE qint64 pid() const;
    Q_SCRIPTABLE void restore();

    KActionCollection *actionCollection() const;

    QAction *quitAction() const;
    QAction *configureAction() const;
    QAction *configureNotificationsAction() const;
    QAction *configureKeyBindingsAction() const;
    QAction *aboutAction() const;
    QAction *reportBugAction() const;
    QAction *startHeadsetControlAction() const;
    QAction *stopHeadsetControlAction() const;
    QAction *refreshHeadsetControlAction() const;

public Q_SLOTS:
    void quit();

Q_SIGNALS:
    void showWindow();
    void showSettings();
    void showAbout();
    void actionsChanged();

private Q_SLOTS:
    void onRunInBackgroundChanged();

private:
    HeadsetKontrolNotifierItem *m_notifierItem;
    KActionCollection *m_actionCollection;

    void initAction();
};

#endif // HEADSETKONTROLAPPLICATION_H
