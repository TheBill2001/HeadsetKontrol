// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKCHATMIX_HPP
#define HKCHATMIX_HPP

#include "headsetkontrol_export.hpp"

#include <QMetaType>

class HK_EXPORT HKChatMix
{
    Q_GADGET
    Q_PROPERTY(quint8 level MEMBER level FINAL)
    Q_PROPERTY(quint8 gameVolumePercent MEMBER gameVolumePercent FINAL)
    Q_PROPERTY(quint8 chatVolumePercent MEMBER chatVolumePercent FINAL)
public:
    quint8 level{64};
    quint8 gameVolumePercent{50};
    quint8 chatVolumePercent{50};
};

[[nodiscard]] constexpr std::partial_ordering operator<=>(const HKChatMix &lhs, const HKChatMix &rhs) noexcept
{
    return lhs.level <=> rhs.level;
}

[[nodiscard]] constexpr bool operator==(const HKChatMix &lhs, const HKChatMix &rhs) noexcept
{
    return lhs.level == rhs.level;
}

template<typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
[[nodiscard]] constexpr std::partial_ordering operator<=>(const HKChatMix &lhs, const T &rhs) noexcept
{
    return lhs.level <=> rhs;
}

template<typename T>
    requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
[[nodiscard]] constexpr bool operator==(const HKChatMix &lhs, const T &rhs) noexcept
{
    return lhs.level == rhs;
}

[[maybe_unused]] HK_EXPORT QDebug operator<<(QDebug debug, const HKChatMix &chatMix);

Q_DECLARE_TYPEINFO(HKChatMix, Q_PRIMITIVE_TYPE);

#endif // HKCHATMIX_HPP
