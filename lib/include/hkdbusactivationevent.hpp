// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKDBUSACTIVATIONEVENT_HPP
#define HKDBUSACTIVATIONEVENT_HPP

#include "headsetkontrol_export.hpp"

#include <QEvent>
#include <QUrl>
#include <QVariant>

class HK_EXPORT HKDBusActivateEvent : public QEvent
{
    Q_GADGET
    Q_PROPERTY(QStringList arguments READ arguments CONSTANT FINAL)
    Q_PROPERTY(QString workingDirectory READ workingDirectory CONSTANT FINAL)
    Q_DECL_EVENT_COMMON(HKDBusActivateEvent)
public:
    static const int eventId;

    HKDBusActivateEvent(QStringList arguments, QString workingDirectory) noexcept;

    [[nodiscard]] QStringList arguments() const noexcept;

    [[nodiscard]] QString workingDirectory() const noexcept;

private:
    QStringList m_arguments;
    QString m_workingDirectory;
};

class HK_EXPORT HKDBusActivateActionEvent : public QEvent
{
    Q_GADGET
    Q_PROPERTY(QString actionName READ actionName CONSTANT FINAL)
    Q_PROPERTY(QVariant parameter READ parameter CONSTANT FINAL)
    Q_DECL_EVENT_COMMON(HKDBusActivateActionEvent)
public:
    static const int eventId;

    HKDBusActivateActionEvent(QString actionName, QVariant parameter) noexcept;

    [[nodiscard]] QString actionName() const noexcept;

    [[nodiscard]] QVariant parameter() const noexcept;

private:
    QString m_actionName;
    QVariant m_parameter;
};

class HK_EXPORT HKDBusOpenEvent : public QEvent
{
    Q_GADGET
    Q_PROPERTY(QList<QUrl> uris READ uris CONSTANT FINAL)
    Q_DECL_EVENT_COMMON(HKDBusOpenEvent)
public:
    static const int eventId;

    explicit HKDBusOpenEvent(QList<QUrl> uris) noexcept;

    [[nodiscard]] QList<QUrl> uris() const noexcept;

private:
    QList<QUrl> m_uris;
};

#endif // HKDBUSACTIVATIONEVENT_HPP
