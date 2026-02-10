// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkchatmix.hpp"

#include <QDebug>

QDebug operator<<(QDebug debug, const HKChatMix &chatMix)
{
    QDebugStateSaver saver(debug);

    return debug.nospace() << "HKChatMix(" << chatMix.m_level << ", " << chatMix.m_gameVolumePercent << ", " << chatMix.m_chatVolumePercent << ")";
}
