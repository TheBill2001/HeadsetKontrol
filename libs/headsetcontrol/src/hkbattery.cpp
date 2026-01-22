// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkbattery.hpp"

#include "hkhcutils_p.hpp"
#include "hkmacros_p.hpp"

#include <QDebug>

#include <KLocalizedString>

HK_ASSERT_COPYABLE_MOVEABLE(HKBattery)

QDebug operator<<(QDebug debug, const HKBattery &battery)
{
    QDebugStateSaver saver(debug);

    debug.noquote().nospace() << "QtHeadsetControl::Battery(" << battery.level << "%, " << HKUtils::enumKey(battery.status) << ", "
                              << HKUtils::enumKey(battery.microphoneStatus);

    if (battery.hasVoltage()) {
        debug << ", " << battery.voltage() << "mV";
    }

    if (battery.hasTimeToEmpty()) {
        debug << ", " << i18ncp("@info:shell time until battery empty debug message", "%1 minute until empty", "%1 minutes until empty", battery.timeToEmpty());
    }

    if (battery.hasTimeToFull()) {
        debug << ", " << i18ncp("@info:shell time until battery full debug message", "%1 minute until full", "%1 minutes until full", battery.timeToFull());
    }

    debug << ")";

    return debug;
}
