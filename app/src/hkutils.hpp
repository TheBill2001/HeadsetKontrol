// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKUTILS_HPP
#define HKUTILS_HPP

#include "headsetkontrol_export.hpp"

#include <QString>

class HKBattery;

namespace HKUtils
{
[[nodiscard]] HK_NO_EXPORT QString batteryIconName(const HKBattery &battery, QAnyStringView defaultIconName = {});

[[nodiscard]] HK_NO_EXPORT QString batteryStatusToLocaleString(const HKBattery &battery);
} // namespace HKUtils

#endif // HKUTILS_HPP
