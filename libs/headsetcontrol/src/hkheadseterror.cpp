// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadseterror.hpp"

#include "hkhcutils_p.hpp"
#include "hkmacros_p.hpp"

HK_ASSERT_COPYABLE_MOVEABLE(HKHeadsetError)

QDebug operator<<(QDebug debug, const HKHeadsetError &error)
{
    QDebugStateSaver saver(debug);

    debug.nospace() << "HKHeadsetError(" << error.headset << ", " << HKUtils::enumKey<HKHeadset::Capabilities>(error.capability) << ", "
                    << HKUtils::enumKey(error.error);

    QString string = error.errorString;
    if (string.isEmpty()) {
        debug << ", " << string;
    }

    string = error.details;
    if (string.isEmpty()) {
        debug << ", " << string;
    }

    debug << ", " << error.timestamp << ")";

    return debug;
}
