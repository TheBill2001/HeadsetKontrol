// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkutils.hpp"

#include "hkbattery.hpp"

#include <KLocalizedString>

using namespace Qt::StringLiterals;

namespace HKUtils
{
HK_NO_EXPORT QString batteryIconName(const HKBattery &battery, QAnyStringView defaultIconName)
{
    if (battery.status > HKBattery::BatteryUnavailable) {
        const auto level = 10 * ((battery.level + 5) / 10);
        QString iconName = u"battery-%1"_s.arg(level, 3, 10, '0'_L1);
        if (battery.status == HKBattery::BatteryCharging) {
            iconName.append(u"-charging"_s);
        }
        return iconName;
    }
    return defaultIconName.toString();
}

QString batteryStatusToLocaleString(const HKBattery &battery)
{
    switch (battery.status) {
    case HKBattery::BatteryHidError:
        return i18nc("@item:intext battery status", "HID error");
    case HKBattery::BatteryTimeout:
        return i18nc("@item:intext battery status", "Timed out");
    case HKBattery::BatteryCharging:
        return i18nc("@item:intext battery status", "Charging");
    case HKBattery::BatteryAvailable:
        return i18nc("@item:intext battery status", "Available");
    case HKBattery::BatteryUnavailable:
        break;
    }
    return i18nc("@item:intext battery status", "Unavailable");
}
} // namespace HKUtils
