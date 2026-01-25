// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkdbusactivationevent.hpp"

Q_IMPL_EVENT_COMMON(HKDBusActivateEvent)
Q_IMPL_EVENT_COMMON(HKDBusActivateActionEvent)
Q_IMPL_EVENT_COMMON(HKDBusOpenEvent)

const int HKDBusActivateEvent::eventId = QEvent::registerEventType();
const int HKDBusActivateActionEvent::eventId = QEvent::registerEventType();
const int HKDBusOpenEvent::eventId = QEvent::registerEventType();

HKDBusActivateEvent::HKDBusActivateEvent(QStringList arguments, QString workingDirectory) noexcept
    : QEvent{static_cast<QEvent::Type>(eventId)}
    , m_arguments{std::move(arguments)}
    , m_workingDirectory{std::move(workingDirectory)}
{
}

QStringList HKDBusActivateEvent::arguments() const noexcept
{
    return m_arguments;
}

QString HKDBusActivateEvent::workingDirectory() const noexcept
{
    return m_workingDirectory;
}

HKDBusActivateActionEvent::HKDBusActivateActionEvent(QString actionName, QVariant parameter) noexcept
    : QEvent{static_cast<QEvent::Type>(eventId)}
    , m_actionName{std::move(actionName)}
    , m_parameter{std::move(parameter)}
{
}

QString HKDBusActivateActionEvent::actionName() const noexcept
{
    return m_actionName;
}

QVariant HKDBusActivateActionEvent::parameter() const noexcept
{
    return m_parameter;
}

HKDBusOpenEvent::HKDBusOpenEvent(QList<QUrl> uris) noexcept
    : QEvent{static_cast<QEvent::Type>(eventId)}
    , m_uris(std::move(uris))
{
}

QList<QUrl> HKDBusOpenEvent::uris() const noexcept
{
    return m_uris;
}
