// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKBATTERY_HPP
#define HKBATTERY_HPP

#include "hkhc_export.hpp"

#include <QMetaType>
#include <QPointer>

class HKHeadset;
class HKHC_EXPORT HKBattery
{
    Q_GADGET
    Q_PROPERTY(quint8 level READ level CONSTANT FINAL)
    Q_PROPERTY(HKBattery::BatteryStatus status READ status CONSTANT FINAL)
    Q_PROPERTY(HKBattery::MicrophoneStatus microphoneStatus READ microphoneStatus CONSTANT FINAL)
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

    constexpr HKBattery() noexcept = default;

    constexpr HKBattery(quint8 level,
                        BatteryStatus status,
                        MicrophoneStatus microphoneStatus,
                        std::optional<int> optionalVoltage = {},
                        std::optional<int> optionalTimeToEmpty = {},
                        std::optional<int> optionalTimeToFull = {}) noexcept
        : m_level{level}
        , m_status{status}
        , m_microphoneStatus{microphoneStatus}
        , m_optionalVoltage(optionalVoltage)
        , m_optionalTimeToEmpty(optionalTimeToEmpty)
        , m_optionalTimeToFull(optionalTimeToFull)
    {
    }

    [[nodiscard]] constexpr quint8 level() const noexcept
    {
        return m_level;
    }

    [[nodiscard]] constexpr BatteryStatus status() const noexcept
    {
        return m_status;
    }

    [[nodiscard]] constexpr MicrophoneStatus microphoneStatus() const noexcept
    {
        return m_microphoneStatus;
    }

    [[nodiscard]] constexpr std::optional<int> optionalVoltage() const noexcept
    {
        return m_optionalVoltage;
    }

    [[nodiscard]] constexpr std::optional<int> optionalTimeToEmpty() const noexcept
    {
        return m_optionalTimeToEmpty;
    }

    [[nodiscard]] constexpr std::optional<int> optionalTimeToFull() const noexcept
    {
        return m_optionalTimeToFull;
    }

    [[nodiscard]] constexpr bool hasVoltage() const noexcept
    {
        return m_optionalVoltage.has_value();
    }

    [[nodiscard]] constexpr int voltage() const noexcept
    {
        return m_optionalVoltage.value_or(0);
    }

    [[nodiscard]] constexpr bool hasTimeToEmpty() const noexcept
    {
        return m_optionalTimeToEmpty.has_value();
    }

    [[nodiscard]] constexpr int timeToEmpty() const noexcept
    {
        return m_optionalTimeToEmpty.value_or(0);
    }

    [[nodiscard]] constexpr bool hasTimeToFull() const noexcept
    {
        return m_optionalTimeToFull.has_value();
    }

    [[nodiscard]] constexpr int timeToFull() const noexcept
    {
        return m_optionalTimeToFull.value_or(0);
    }

    friend constexpr std::partial_ordering operator<=>(const HKBattery &lhs, const HKBattery &rhs) noexcept;
    friend constexpr bool operator==(const HKBattery &lhs, const HKBattery &rhs) noexcept;
    friend QDebug operator<<(QDebug debug, const HKBattery &battery);

private:
    quint8 m_level{0};
    BatteryStatus m_status{BatteryUnavailable};
    MicrophoneStatus m_microphoneStatus{MicrophoneUnknown};

    std::optional<int> m_optionalVoltage;
    std::optional<int> m_optionalTimeToEmpty;
    std::optional<int> m_optionalTimeToFull;
};

Q_DECLARE_TYPEINFO(HKBattery, Q_PRIMITIVE_TYPE);

[[nodiscard]] constexpr std::partial_ordering operator<=>(const HKBattery &lhs, const HKBattery &rhs) noexcept
{
    if (lhs.m_status <= HKBattery::BatteryUnavailable && rhs.m_status <= HKBattery::BatteryUnavailable) {
        return lhs.m_status <=> rhs.m_status;
    }
    if (lhs.m_status <= HKBattery::BatteryUnavailable) {
        return std::partial_ordering::less;
    }
    if (rhs.m_status <= HKBattery::BatteryUnavailable) {
        return std::partial_ordering::greater;
    }
    return lhs.m_level <=> rhs.m_level;
}

[[nodiscard]] constexpr bool operator==(const HKBattery &lhs, const HKBattery &rhs) noexcept
{
    return lhs.m_level == rhs.m_level && lhs.m_status == rhs.m_status && lhs.m_microphoneStatus == rhs.m_microphoneStatus
        && lhs.m_optionalVoltage == rhs.m_optionalVoltage && lhs.m_optionalTimeToEmpty == rhs.m_optionalTimeToEmpty
        && lhs.m_optionalTimeToFull == rhs.m_optionalTimeToFull;
}

[[maybe_unused]] HKHC_EXPORT QDebug operator<<(QDebug debug, const HKBattery &battery);

#endif // HKBATTERY_HPP
