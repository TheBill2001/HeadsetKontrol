// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETID_HPP
#define HKHEADSETID_HPP

#include "headsetkontrol_export.hpp"

#include <QMetaType>

class HK_EXPORT HKHeadsetId
{
    Q_GADGET
    Q_PROPERTY(quint16 vendorId MEMBER vendorId FINAL)
    Q_PROPERTY(quint16 productId MEMBER productId FINAL)
public:
    quint16 vendorId{0};
    quint16 productId{0};

    constexpr HKHeadsetId() noexcept = default;

    constexpr HKHeadsetId(quint16 vendorId, quint16 productId) noexcept
        : vendorId{vendorId}
        , productId{productId}
    {
    }

    constexpr HKHeadsetId(std::pair<quint16, quint16> other) noexcept // NOLINT(*explicit-conversions)
        : vendorId{std::move(other).first}
        , productId{std::move(other).second}
    {
    }

    HKHeadsetId(QAnyStringView vendorId, QAnyStringView productId);

    explicit HKHeadsetId(QAnyStringView HKHeadsetId);

    Q_INVOKABLE explicit HKHeadsetId(const QString &headsetId)
        : HKHeadsetId{QAnyStringView{headsetId}}
    {
    }

    Q_INVOKABLE constexpr HKHeadsetId(quint32 HKHeadsetId) noexcept // NOLINT(*explicit-conversions)
        : vendorId{quint16(HKHeadsetId >> 16U)}
        , productId{quint16(HKHeadsetId & 0xFFFFU)}
    {
    }

    Q_INVOKABLE [[nodiscard]] constexpr bool isValid() const noexcept
    {
        return vendorId != 0 && productId != 0;
    }

    HKHeadsetId &operator=(std::pair<quint16, quint16> other) noexcept
    {
        vendorId = std::move(other).first;
        productId = std::move(other).second;
        return *this;
    }

    Q_INVOKABLE [[nodiscard]] QString toString() const;

    [[nodiscard]] operator QString() const // NOLINT(*explicit-conversions)
    {
        return toString();
    }
};

[[nodiscard]] constexpr std::strong_ordering operator<=>(const HKHeadsetId &lhs, const HKHeadsetId &rhs) noexcept
{
    auto order = lhs.vendorId <=> rhs.vendorId;
    if (order == std::strong_ordering::equal) {
        order = lhs.productId <=> rhs.productId;
    }
    return order;
}

[[nodiscard]] constexpr bool operator==(const HKHeadsetId &lhs, const HKHeadsetId &rhs) noexcept
{
    return lhs.vendorId == rhs.vendorId && lhs.productId == rhs.productId;
}

[[nodiscard]] constexpr std::strong_ordering operator<=>(const HKHeadsetId &lhs, const std::pair<quint16, quint16> &rhs) noexcept
{
    auto order = lhs.vendorId <=> rhs.first;
    if (order == std::strong_ordering::equal) {
        order = lhs.productId <=> rhs.second;
    }
    return order;
}

[[nodiscard]] constexpr bool operator==(const HKHeadsetId &lhs, const std::pair<quint16, quint16> &rhs) noexcept
{
    return lhs.vendorId == rhs.first && lhs.productId == rhs.second;
}

[[nodiscard]] HK_EXPORT std::partial_ordering operator<=>(const HKHeadsetId &lhs, QAnyStringView rhs) noexcept;
[[nodiscard]] HK_EXPORT bool operator==(const HKHeadsetId &lhs, QAnyStringView rhs) noexcept;

[[maybe_unused]] HK_EXPORT QDebug operator<<(QDebug debug, const HKHeadsetId &HKHeadsetId);

namespace std
{
template<>
struct hash<HKHeadsetId> {
    constexpr std::size_t operator()(const HKHeadsetId &HKHeadsetId, std::size_t /*seed*/) const noexcept
    {
        return (quint32(HKHeadsetId.vendorId) << 16U) | quint32(HKHeadsetId.productId);
    }
};
} // namespace std

Q_DECLARE_TYPEINFO(HKHeadsetId, Q_PRIMITIVE_TYPE);

#endif // HKHEADSETID_HPP
