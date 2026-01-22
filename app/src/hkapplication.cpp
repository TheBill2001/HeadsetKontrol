// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkapplication.hpp"

#include "hkdbusactivationevent.hpp"
#include "hklogging_p.hpp"

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTimer>

#include <KLocalizedString>
#include <KWindowSystem>

HKApplication::HKApplication(QObject *parent)
    : AbstractKirigamiApplication{parent}
{
    HKApplication::setupActions();

    QCoreApplication::instance()->installEventFilter(this);

    QTimer::singleShot(0, this, &HKApplication::detectWindow);
}

QAction *HKApplication::quitAction()
{
    return action(KStandardActions::name(KStandardActions::Quit));
}

QBindable<QWindow *> HKApplication::bindableWindow()
{
    return &m_window;
}

QWindow *HKApplication::window() const
{
    return m_window.value();
}

void HKApplication::setWindow(QWindow *window)
{
    m_window = window;
}

bool HKApplication::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::ParentChange:
        detectWindow();
        QObject::event(event);
        return true;
    default:
        break;
    }
    return QObject::event(event);
}

bool HKApplication::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == QCoreApplication::instance()) {
        if (event->type() == HKDBusActivateEvent::eventId) {
            if (auto *activateEvent = dynamic_cast<HKDBusActivateEvent *>(event)) {
                activateEvent->accept();

                if (auto window = m_window.value()) {
                    window->show();
                    KWindowSystem::updateStartupId(window);
                    window->raise();
                    KWindowSystem::activateWindow(window);
                }

                return true;
            } else {
                Q_UNLIKELY_BRANCH;
                qCDebug(HK_LOGGING).noquote() << i18nc("@info:shell main app event filter", "Invalid HKDBusActivateEvent object.");
                return false;
            }
        }
    }
    return AbstractKirigamiApplication::eventFilter(watched, event);
}

void HKApplication::setupActions()
{
    AbstractKirigamiApplication::setupActions();

    readSettings();
}

void HKApplication::detectWindow()
{
    if (auto *quickItem = qobject_cast<QQuickItem *>(parent())) {
        m_window = quickItem->window();
    } else {
        if (auto *engine = qobject_cast<QQmlApplicationEngine *>(qmlEngine(this))) {
            const auto rootObjects = engine->rootObjects();
            if (rootObjects.isEmpty()) {
                return;
            }
            if (auto *window = qobject_cast<QQuickWindow *>(rootObjects.first())) {
                m_window = window;
            }
        }
    }
}
