// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkbattery.hpp"

#include "hkhcutils_p.hpp"
#include "hkmacros_p.hpp"

#include <QDebug>

#include <KLocalizedString>

using namespace Qt::StringLiterals;

HK_ASSERT_COPYABLE_MOVEABLE(HKBattery)

QString HKBattery::iconName(bool styled, const QString &fallback) const
{
    if (status > HKBattery::BatteryUnavailable) {
        const auto level = 10 * ((this->level + 5) / 10);
        QString iconName = u"battery-%1"_s.arg(level, 3, 10, '0'_L1);
        if (styled) {
            iconName = u"headsetkontrol-"_s + iconName;
        }
        if (status == HKBattery::BatteryCharging) {
            iconName.append(u"-charging"_s);
        }
        return iconName;
    }
    return fallback;
}

QString HKBattery::batteryStatusToLocaleString() const
{
    return batteryStatusToLocaleString(status);
}

QString HKBattery::batteryStatusToLocaleString(BatteryStatus status)
{
    switch (status) {
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

QDebug operator<<(QDebug debug, const HKBattery &battery)
{
    QDebugStateSaver saver(debug);

    debug.noquote().nospace() << "QtHeadsetControl::Battery(" << battery.level << "%, " << HKUtils::enumKey(battery.status) << ", "
                              << HKUtils::enumKey(battery.microphoneStatus);

    if (battery.hasVoltage()) {
        debug << ", " << battery.voltage() << "mV";
    }

    if (battery.hasTimeToEmpty()) {
        debug << ", " << battery.timeToEmpty() << " minute(s) until empty";
    }

    if (battery.hasTimeToFull()) {
        debug << ", " << battery.timeToFull() << "%1 minute(s) until full";
    }

    debug << ")";

    return debug;
}
