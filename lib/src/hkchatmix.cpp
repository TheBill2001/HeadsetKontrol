// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkchatmix.hpp"

#include "hkmacros_p.hpp"

#include <QDebug>

HK_ASSERT_COPYABLE_MOVEABLE(HKChatMix)

QDebug operator<<(QDebug debug, const HKChatMix &chatMix)
{
    QDebugStateSaver saver(debug);

    return debug.nospace() << "QtHeadsetControl::ChatMix(" << chatMix.level << ", " << chatMix.gameVolumePercent << ", " << chatMix.chatVolumePercent << ")";
}
