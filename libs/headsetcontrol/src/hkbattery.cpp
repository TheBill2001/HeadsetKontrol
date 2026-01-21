// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkbattery.hpp"

#include "hkhcutils_p.hpp"

#include <QDebug>

QDebug operator<<(QDebug debug, const HKBattery &battery)
{
    QDebugStateSaver saver(debug);

    debug.noquote().nospace() << "QtHeadsetControl::Battery(" << battery.level << "%, " << HKUtils::enumKey(battery.status) << ", "
                              << HKUtils::enumKey(battery.microphoneStatus);

    if (battery.hasVoltage()) {
        debug << ", " << battery.voltage() << "mV";
    }

    if (battery.hasTimeToFull()) {
        debug << ", " << battery.timeToFull() << " minute(s) until full";
    }

    if (battery.hasTimeToEmpty()) {
        debug << ", " << battery.timeToEmpty() << " minute(s) until empty";
    }

    debug << ")";

    return debug;
}
