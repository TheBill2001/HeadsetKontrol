// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKCHATMIX_HPP
#define HKCHATMIX_HPP

#include <QMetaType>

class HKChatMix
{
    Q_GADGET
    Q_PROPERTY(quint8 level READ level CONSTANT FINAL)
    Q_PROPERTY(quint8 gameVolumePercent READ gameVolumePercent CONSTANT FINAL)
    Q_PROPERTY(quint8 chatVolumePercent READ chatVolumePercent CONSTANT FINAL)
public:
    constexpr HKChatMix() noexcept = default;

    constexpr HKChatMix(quint8 level, quint8 gameVolumePercent, quint8 chatVolumePercent) noexcept
        : m_level{level}
        , m_gameVolumePercent{gameVolumePercent}
        , m_chatVolumePercent{chatVolumePercent}
    {
    }

    [[nodiscard]] quint8 level() const noexcept
    {
        return m_level;
    }

    [[nodiscard]] quint8 gameVolumePercent() const noexcept
    {
        return m_gameVolumePercent;
    }

    [[nodiscard]] quint8 chatVolumePercent() const noexcept
    {
        return m_chatVolumePercent;
    }

    friend constexpr std::partial_ordering operator<=>(const HKChatMix &lhs, const HKChatMix &rhs) noexcept;
    friend constexpr bool operator==(const HKChatMix &lhs, const HKChatMix &rhs) noexcept;

    template<typename T>
        requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
    friend constexpr std::partial_ordering operator<=>(const HKChatMix &lhs, const T &rhs) noexcept;

    template<typename T>
        requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
    friend constexpr bool operator==(const HKChatMix &lhs, const T &rhs) noexcept;

    friend QDebug operator<<(QDebug debug, const HKChatMix &chatMix);

private:
    quint8 m_level{64};
    quint8 m_gameVolumePercent{100};
    quint8 m_chatVolumePercent{100};
};

Q_DECLARE_TYPEINFO(HKChatMix, Q_PRIMITIVE_TYPE);

[[nodiscard]] constexpr std::partial_ordering operator<=>(const HKChatMix &lhs, const HKChatMix &rhs) noexcept
{
    return lhs.m_level <=> rhs.m_level;
}

[[nodiscard]] constexpr bool operator==(const HKChatMix &lhs, const HKChatMix &rhs) noexcept
{
    return lhs.m_level == rhs.m_level;
}

template<typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
[[nodiscard]] constexpr std::partial_ordering operator<=>(const HKChatMix &lhs, const T &rhs) noexcept
{
    return lhs.m_level <=> rhs;
}

template<typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
[[nodiscard]] constexpr bool operator==(const HKChatMix &lhs, const T &rhs) noexcept
{
    return lhs.m_level == rhs;
}

[[maybe_unused]] QDebug operator<<(QDebug debug, const HKChatMix &chatMix);

#endif // HKCHATMIX_HPP
