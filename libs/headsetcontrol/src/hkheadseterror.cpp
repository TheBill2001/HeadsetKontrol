// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadseterror.hpp"

#include "hkcommonutils.hpp"

using namespace Qt::StringLiterals;

QDebug operator<<(QDebug debug, const HKHeadsetError &error)
{
    QDebugStateSaver saver(debug);

    debug.nospace() << "HKHeadsetError(" << error.m_headset << ", " << HKUtils::flagKey(error.m_capability) << ", " << HKUtils::enumKey(error.m_error);

    if (error.m_errorString.isEmpty()) {
        debug << ", " << error.m_errorString;
    }

    if (error.m_details.isEmpty()) {
        debug << ", " << error.m_details;
    }

    debug << ", " << error.m_timestamp << ")";

    return debug;
}
