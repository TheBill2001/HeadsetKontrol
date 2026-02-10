// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKCLAMPEDNUMBER_HPP
#define HKCLAMPEDNUMBER_HPP

#include "hkglobal.hpp"

template<HKNumeric T, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()>
    requires std::totally_ordered<T>
class HKClampedNumber
{
    T m_value;

    template<HKNumeric V>
        requires std::totally_ordered<T>
    [[nodiscard]] static T clamp(const V &value)
    {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }

public:
    constexpr HKClampedNumber() noexcept
        : m_value{0}
    {
    }

    template<HKNumeric V>
        requires std::totally_ordered<T>
    constexpr HKClampedNumber(const V &value) noexcept // NOLINT(*explicit-conversions)
        : m_value{clamp(value)}
    {
    }

    constexpr HKClampedNumber(const HKClampedNumber &other) noexcept
        : m_value(other.m_value)
    {
    }

    constexpr HKClampedNumber(HKClampedNumber &&other) noexcept
        : m_value{std::move(other).m_value}
    {
    }

    template<HKNumeric V>
        requires std::totally_ordered<T>
    [[nodiscard]] HKClampedNumber &operator=(const V &value) noexcept
    {
        m_value = clamp(value);
    }

    [[nodiscard]] HKClampedNumber &operator=(const HKClampedNumber &other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        m_value = other.m_value;
        return *this;
    }

    [[nodiscard]] HKClampedNumber &operator=(HKClampedNumber &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        m_value = std::move(other).m_value;
        return *this;
    }

    [[nodiscard]] constexpr operator T() const noexcept // NOLINT(*explicit-conversions)
    {
        return m_value;
    }
};

template<HKNumeric T, HKNumeric V>
constexpr std::strong_ordering operator<=>(const HKClampedNumber<T> &lhs, const HKClampedNumber<V> &rhs) noexcept
{
    return lhs.m_value <=> rhs.m_value;
}

template<HKNumeric T, HKNumeric V>
constexpr bool operator==(const HKClampedNumber<T> &lhs, const HKClampedNumber<V> &rhs) noexcept
{
    return lhs.m_value == rhs.m_value;
}

template<HKNumeric T, HKNumeric V>
constexpr std::strong_ordering operator<=>(const HKClampedNumber<T> &lhs, const V &rhs) noexcept
{
    return lhs.m_value <=> rhs;
}

template<HKNumeric T, HKNumeric V>
constexpr bool operator==(const HKClampedNumber<T> &lhs, const V &rhs) noexcept
{
    return lhs.m_value == rhs;
}

#endif // HKCLAMPEDNUMBER_HPP
