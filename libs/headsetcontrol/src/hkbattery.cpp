// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkbattery.hpp"

#include "hkcommonutils.hpp"

#include <QDebug>

using namespace Qt::StringLiterals;

QDebug operator<<(QDebug debug, const HKBattery &battery)
{
    QDebugStateSaver saver(debug);

    debug.noquote().nospace() << "QtHeadsetControl::Battery(" << battery.m_level << "%, " << HKUtils::enumKey(battery.m_status) << ", "
                              << HKUtils::enumKey(battery.m_microphoneStatus);

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
