// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKOBJECTPRIVATE_P_HPP
#define HKOBJECTPRIVATE_P_HPP

#include "headsetkontrol_export.hpp"

#include <QObject>

class HK_NO_EXPORT HKObjectPrivate
{
    Q_DISABLE_COPY(HKObjectPrivate)
protected:
    QObject *const q_ptr;

public:
    explicit HKObjectPrivate(QObject *const q_ptr);

    friend QBindingStorage *qGetBindingStorage(HKObjectPrivate *const object);
    friend const QBindingStorage *qGetBindingStorage(const HKObjectPrivate *const object);
};

[[nodiscard]] inline QBindingStorage *qGetBindingStorage(HKObjectPrivate *object)
{
    return object->q_ptr->bindingStorage();
}

[[nodiscard]] inline const QBindingStorage *qGetBindingStorage(const HKObjectPrivate *const object)
{
    return static_cast<const QObject *>(object->q_ptr)->bindingStorage();
}

#endif // HKOBJECTPRIVATE_P_HPP
