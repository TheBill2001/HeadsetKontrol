// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKUTILS_HPP
#define HKUTILS_HPP

#include "hkapp_export.hpp"

#include "hkbattery.hpp"
#include "hkheadset.hpp"
#include "hkheadseterror.hpp"

class QWindow;

namespace HKUtils
{
HKAPP_EXPORT void raiseWindow(QWindow *window);

[[nodiscard]] HKAPP_EXPORT QString batteryIconName(const HKBattery &battery, bool styled = false, const QString &fallback = {});
[[nodiscard]] HKAPP_EXPORT QString batteryStatusToLocaleString(HKBattery::BatteryStatus status);

[[nodiscard]] HKAPP_EXPORT QString capabilityToLocaleString(HKHeadset::Capability capability);
[[nodiscard]] HKAPP_EXPORT QStringList capabilitiesToLocaleStrings(HKHeadset::Capabilities capabilities);

[[nodiscard]] HKAPP_EXPORT QString headsetErrorLocaleErrorString(const HKHeadsetError &error);
[[nodiscard]] HKAPP_EXPORT QString headsetErrorToLocaleString(HKHeadsetError::Error error);
} // namespace HKUtils

#endif // HKUTILS_HPP
