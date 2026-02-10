// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKUTILS_HPP
#define HKUTILS_HPP

#include <QMetaEnum>

namespace HKUtils
{
template<typename T, auto min = std::numeric_limits<T>::min(), auto max = std::numeric_limits<T>::max()>
[[nodiscard]] constexpr T clamp(auto &&value) noexcept
{
    if (value < min) {
        return std::forward<decltype(min)>(min);
    }
    if (value > max) {
        return std::forward<decltype(max)>(max);
    }
    return static_cast<T>(std::forward<decltype(value)>(value));
}

template<typename T>
[[nodiscard]] const char *enumKey(T value)
{
    return QMetaEnum::fromType<T>().valueToKey(value);
}

template<typename T>
[[nodiscard]] const char *flagKey(T value)
{
    return QMetaEnum::fromType<QFlags<T>>().valueToKey(value);
}

template<typename Enum>
[[nodiscard]] qsizetype countFlags(QFlags<Enum> flags)
{
    qsizetype count = 0;
    const auto metaEnum = QMetaEnum::fromType<QFlags<Enum>>();
    if (metaEnum.is64Bit()) {
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            const std::optional<quint64> valueOpt = metaEnum.value64(i);
            if (valueOpt.has_value()) {
                if (flags.testFlag(static_cast<Enum>(valueOpt.value()))) {
                    ++count;
                }
            }
        }
    } else {
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            if (flags.testFlag(static_cast<Enum>(metaEnum.value(i)))) {
                ++count;
            }
        }
    }
    return count;
}

// template<typename Enum>
// [[nodiscard]] QFlags<Enum> allFlags()
// {
//     QFlags<Enum> flags;
//     const auto metaEnum = QMetaEnum::fromType<QFlags<Enum>>();
//     if (metaEnum.is64Bit()) {
//         for (int i = 0; i < metaEnum.keyCount(); ++i) {
//             const std::optional<quint64> valueOpt = metaEnum.value64(i);
//             if (valueOpt.has_value()) {
//                 flags.setFlag(static_cast<Enum>(valueOpt.value()));
//             }
//         }
//     } else {
//         for (int i = 0; i < metaEnum.keyCount(); ++i) {
//             flags.setFlag(static_cast<Enum>(metaEnum.value(i)));
//         }
//     }
//     return flags;
// }
} // namespace HKUtils

#endif // HKUTILS_HPP
