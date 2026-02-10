// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETID_HPP
#define HKHEADSETID_HPP

#include <QMetaType>

class HKHeadsetId
{
    Q_GADGET
    Q_PROPERTY(quint16 vendorId READ vendorId CONSTANT FINAL)
    Q_PROPERTY(quint16 productId READ productId CONSTANT FINAL)
public:
    constexpr HKHeadsetId() noexcept = default;

    constexpr HKHeadsetId(quint16 vendorId, quint16 productId) noexcept
        : m_vendorId{vendorId}
        , m_productId{productId}
    {
    }

    constexpr HKHeadsetId(std::pair<quint16, quint16> other) noexcept // NOLINT(*explicit-conversions)
        : m_vendorId{std::move(other).first}
        , m_productId{std::move(other).second}
    {
    }

    Q_INVOKABLE constexpr HKHeadsetId(quint32 headsetId) noexcept // NOLINT(*explicit-conversions)
        : m_vendorId{quint16(headsetId >> 16U)}
        , m_productId{quint16(headsetId & 0xFFFFU)}
    {
    }

    HKHeadsetId(QAnyStringView vendorId, QAnyStringView productId);

    explicit HKHeadsetId(QAnyStringView headsetId);

    Q_INVOKABLE explicit HKHeadsetId(const QString &headsetId)
        : HKHeadsetId{QAnyStringView{headsetId}}
    {
    }

    Q_INVOKABLE [[nodiscard]] constexpr bool isValid() const noexcept
    {
        return m_vendorId != 0 && m_productId != 0;
    }

    [[nodiscard]] constexpr quint16 vendorId() const noexcept
    {
        return m_vendorId;
    }

    [[nodiscard]] constexpr quint16 productId() const noexcept
    {
        return m_productId;
    }

    HKHeadsetId &operator=(std::pair<quint16, quint16> other) noexcept
    {
        m_vendorId = std::move(other).first;
        m_productId = std::move(other).second;
        return *this;
    }

    Q_INVOKABLE [[nodiscard]] QString toString() const;

    [[nodiscard]] operator QString() const // NOLINT(*explicit-conversions)
    {
        return toString();
    }

    [[nodiscard]] constexpr operator quint32() const noexcept // NOLINT(*explicit-conversions)
    {
        return (quint32(m_vendorId) << 16U) | quint32(m_productId);
    }

    friend constexpr std::strong_ordering operator<=>(const HKHeadsetId &lhs, const HKHeadsetId &rhs) noexcept;
    friend constexpr bool operator==(const HKHeadsetId &lhs, const HKHeadsetId &rhs) noexcept;
    friend constexpr std::strong_ordering operator<=>(const HKHeadsetId &lhs, const std::pair<quint16, quint16> &rhs) noexcept;
    friend constexpr bool operator==(const HKHeadsetId &lhs, const std::pair<quint16, quint16> &rhs) noexcept;

private:
    quint16 m_vendorId{0};
    quint16 m_productId{0};
};

Q_DECLARE_TYPEINFO(HKHeadsetId, Q_PRIMITIVE_TYPE);

[[nodiscard]] constexpr std::strong_ordering operator<=>(const HKHeadsetId &lhs, const HKHeadsetId &rhs) noexcept
{
    auto order = lhs.m_vendorId <=> rhs.m_vendorId;
    if (order == std::strong_ordering::equal) {
        order = lhs.m_productId <=> rhs.m_productId;
    }
    return order;
}

[[nodiscard]] constexpr bool operator==(const HKHeadsetId &lhs, const HKHeadsetId &rhs) noexcept
{
    return lhs.m_vendorId == rhs.m_vendorId && lhs.m_productId == rhs.m_productId;
}

[[nodiscard]] constexpr std::strong_ordering operator<=>(const HKHeadsetId &lhs, const std::pair<quint16, quint16> &rhs) noexcept
{
    auto order = lhs.m_vendorId <=> rhs.first;
    if (order == std::strong_ordering::equal) {
        order = lhs.m_productId <=> rhs.second;
    }
    return order;
}

[[nodiscard]] constexpr bool operator==(const HKHeadsetId &lhs, const std::pair<quint16, quint16> &rhs) noexcept
{
    return lhs.m_vendorId == rhs.first && lhs.m_productId == rhs.second;
}

[[nodiscard]] std::partial_ordering operator<=>(const HKHeadsetId &lhs, QAnyStringView rhs) noexcept;
[[nodiscard]] bool operator==(const HKHeadsetId &lhs, QAnyStringView rhs) noexcept;

[[maybe_unused]] QDebug operator<<(QDebug debug, const HKHeadsetId &HKHeadsetId);

namespace std
{
template<>
struct hash<HKHeadsetId> {
    constexpr std::size_t operator()(const HKHeadsetId &headsetId, std::size_t /*seed*/) const noexcept
    {
        return quint32(headsetId);
    }
};
} // namespace std

#endif // HKHEADSETID_HPP
