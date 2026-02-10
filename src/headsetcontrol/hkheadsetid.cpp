// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadsetid.hpp"

#include "hkglobal.hpp"

using namespace Qt::StringLiterals;

namespace
{
quint16 parseId(QAnyStringView idStringView)
{
    if (idStringView.isEmpty()) {
        return 0;
    }

    bool check = false;
    const auto idString = idStringView.toString();
    auto value = idString.simplified().toLower().toUInt(&check, 16);

    if (!check) {
        HK_WARN("Failed to parse device ID string: \"%s\"", qPrintable(idString));
        return 0;
    }

    if (value == 0 || value > std::numeric_limits<quint16>::max()) {
        HK_WARN("Parsed ID string is out of range: \"%s\"", qPrintable(idString));
        return 0;
    }

    return value;
}

std::pair<quint16, quint16> parseIds(QAnyStringView headsetId)
{
    if (headsetId.isEmpty()) {
        return std::make_pair<quint16, quint16>(0, 0);
    }

    const auto headsetIdString = headsetId.toString();
    const auto strings = headsetIdString.split(':'_L1, Qt::SkipEmptyParts, Qt::CaseInsensitive);
    if (strings.size() != 2) {
        return std::make_pair<quint16, quint16>(0, 0);
    }

    bool check1 = false;
    bool check2 = true;
    const auto vendorId = strings[0].simplified().toLower().toUInt(&check1, 16);
    const auto productId = strings[1].simplified().toLower().toUInt(&check2, 16);

    if (!check1 || !check2) {
        HK_WARN("Failed to parse device ID string: \"%s\"", qPrintable(headsetIdString));
        return std::make_pair<quint16, quint16>(0, 0);
    }

    if (vendorId == 0 || vendorId > std::numeric_limits<quint16>::max()) {
        HK_WARN("Vendor ID is out of range for device ID string: \"%s\"", qPrintable(headsetIdString));
        return std::make_pair<quint16, quint16>(0, 0);
    }

    if (productId == 0 || productId > std::numeric_limits<quint16>::max()) {
        HK_WARN("Product ID is out of range for device ID string: \"%s\"", qPrintable(headsetIdString));
        return std::make_pair<quint16, quint16>(0, 0);
    }

    return std::make_pair<quint16, quint16>(vendorId, productId);
}
}

HKHeadsetId::HKHeadsetId(QAnyStringView vendorId, QAnyStringView productId)
    : HKHeadsetId{parseId(vendorId), parseId(productId)}
{
}

HKHeadsetId::HKHeadsetId(QAnyStringView headsetId)
    : HKHeadsetId{parseIds(headsetId)}
{
}

QString HKHeadsetId::toString() const
{
    return u"0x%1:0x%2"_s.arg(QString::number(m_vendorId, 16), QString::number(m_productId, 16));
}

QDebug operator<<(QDebug debug, const HKHeadsetId &headsetId)
{
    QDebugStateSaver saver(debug);
    return debug.noquote().nospace() << "HKHeadsetId(" << headsetId.toString() << ")";
}

std::partial_ordering operator<=>(const HKHeadsetId &lhs, QAnyStringView rhs) noexcept
{
    HKHeadsetId rhsOther{rhs};
    if (!rhsOther.isValid()) {
        return std::partial_ordering::less;
    }
    return lhs <=> rhsOther;
}

bool operator==(const HKHeadsetId &lhs, QAnyStringView rhs) noexcept
{
    return lhs == HKHeadsetId{rhs};
}
