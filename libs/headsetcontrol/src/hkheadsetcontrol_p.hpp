// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETCONTROL_P_HPP
#define HKHEADSETCONTROL_P_HPP

#include "hkheadsetcontrol.hpp"

#include "hkclampednumber_p.hpp"
#include "hkmacros_p.hpp"

#include <QMutex>
#include <QObjectBindableProperty>
#include <QTimer>

class HKHeadsetControlPrivate
{
    Q_DISABLE_COPY(HKHeadsetControlPrivate)
    Q_DECLARE_PUBLIC(HKHeadsetControl)
    HKHeadsetControl *const q_ptr;

public:
    QMutex internalMutex;

    const QString version;
    const QStringList supportedDevices;

    using UpdateRateValue = HKClampedNumber<int, 0, std::numeric_limits<int>::max()>;
    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadsetControl, UpdateRateValue, updateRate) // NOLINT(clazy-incorrect-emit)

    using DeviceTimeoutValue = HKClampedNumber<int, 0, std::numeric_limits<int>::max()>;
    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadsetControl, DeviceTimeoutValue, deviceTimeout) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_TRIVIAL_PROPERTY(HKHeadsetControl, bool, testDeviceEnabled) // NOLINT(clazy-incorrect-emit)

    using TestProfileValue = HKClampedNumber<int, 0, 10>;
    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadsetControl, TestProfileValue, testProfile) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_TRIVIAL_PROPERTY(HKHeadsetControl, bool, discoverAll) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_TRIVIAL_PROPERTY(HKHeadsetControl, bool, running) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadsetControl, QList<HKHeadset *>, headsets) // NOLINT(clazy-incorrect-emit)

    QTimer timer;
    std::atomic<bool> updating{false};

    explicit HKHeadsetControlPrivate(HKHeadsetControl *const q_ptr);

    void refresh();

    friend QBindingStorage *qGetBindingStorage(HKHeadsetControlPrivate *const object);
    friend const QBindingStorage *qGetBindingStorage(const HKHeadsetControlPrivate *const object);
};

[[nodiscard]] Q_ALWAYS_INLINE QBindingStorage *qGetBindingStorage(HKHeadsetControlPrivate *const object)
{
    return object->q_ptr->bindingStorage();
}

[[nodiscard]] Q_ALWAYS_INLINE const QBindingStorage *qGetBindingStorage(const HKHeadsetControlPrivate *const object)
{
    return static_cast<const HKHeadsetControl *>(object->q_ptr)->bindingStorage();
}

#endif // HKHEADSETCONTROL_P_HPP
