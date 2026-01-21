// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKBATTERY_HPP
#define HKBATTERY_HPP

#include <QMetaType>
#include <QPointer>

class HKHeadset;
class HKBattery
{
    Q_GADGET
    Q_PROPERTY(quint8 level MEMBER level FINAL)
    Q_PROPERTY(HKBattery::BatteryStatus status MEMBER status FINAL)
    Q_PROPERTY(HKBattery::MicrophoneStatus microphoneStatus MEMBER microphoneStatus FINAL)
    Q_PROPERTY(bool hasVoltage READ hasVoltage CONSTANT FINAL)
    Q_PROPERTY(int voltage READ voltage CONSTANT FINAL)
    Q_PROPERTY(bool hasTimeToFull READ hasTimeToFull CONSTANT FINAL)
    Q_PROPERTY(int timeToFull READ timeToFull CONSTANT FINAL)
    Q_PROPERTY(bool hasTimeToEmpty READ hasTimeToEmpty CONSTANT FINAL)
    Q_PROPERTY(int timeToEmpty READ timeToEmpty CONSTANT FINAL)
public:
    enum BatteryStatus : qint8 {
        BatteryHidError = -2,
        BatteryTimeout = -1,
        BatteryUnavailable = 0,
        BatteryAvailable = 1,
        BatteryCharging = 2,
    };
    Q_ENUM(BatteryStatus)

    enum MicrophoneStatus : quint8 {
        MicrophoneUnknown = 0,
        MicrophoneUp
    };
    Q_ENUM(MicrophoneStatus)

    quint8 level{0};
    BatteryStatus status{BatteryUnavailable};
    MicrophoneStatus microphoneStatus{MicrophoneUnknown};

    std::optional<int> optionalVoltage;
    std::optional<int> optionalTimeToEmpty;
    std::optional<int> optionalTimeToFull;

    [[nodiscard]] constexpr bool hasVoltage() const noexcept
    {
        return optionalVoltage.has_value();
    }

    [[nodiscard]] constexpr int voltage() const noexcept
    {
        return optionalVoltage.value_or(0);
    }

    [[nodiscard]] constexpr bool hasTimeToEmpty() const noexcept
    {
        return optionalTimeToEmpty.has_value();
    }

    [[nodiscard]] constexpr int timeToEmpty() const noexcept
    {
        return optionalTimeToEmpty.value_or(0);
    }

    [[nodiscard]] constexpr bool hasTimeToFull() const noexcept
    {
        return optionalTimeToFull.has_value();
    }

    [[nodiscard]] constexpr int timeToFull() const noexcept
    {
        return optionalTimeToFull.value_or(0);
    }
};

[[nodiscard]] constexpr std::partial_ordering operator<=>(const HKBattery &lhs, const HKBattery &rhs) noexcept
{
    if (lhs.status <= HKBattery::BatteryUnavailable && rhs.status <= HKBattery::BatteryUnavailable) {
        return lhs.status <=> rhs.status;
    }
    if (lhs.status <= HKBattery::BatteryUnavailable) {
        return std::partial_ordering::less;
    }
    if (rhs.status <= HKBattery::BatteryUnavailable) {
        return std::partial_ordering::greater;
    }
    return lhs.level <=> rhs.level;
}

[[nodiscard]] constexpr bool operator==(const HKBattery &lhs, const HKBattery &rhs) noexcept
{
    return lhs.level == rhs.level && lhs.status == rhs.status && lhs.microphoneStatus == rhs.microphoneStatus && lhs.optionalVoltage == rhs.optionalVoltage
        && lhs.optionalTimeToEmpty == rhs.optionalTimeToEmpty && lhs.optionalTimeToFull == rhs.optionalTimeToFull;
}

[[maybe_unused]] QDebug operator<<(QDebug debug, const HKBattery &battery);

#endif // HKBATTERY_HPP
