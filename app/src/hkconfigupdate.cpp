// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkconfigupdate.hpp"

using namespace Qt::StringLiterals;

namespace HKConfigUpdate
{
extern int update_2_1_0_main_config();

UpdateInfo update(UpdateId updateId) noexcept
{
    switch (updateId) {
    case HK_2_1_0_MAIN_CONFIG:
        return {.id = updateId, .idString = u"headsetkontrol-2.1.0-main-config"_s, .update = &update_2_1_0_main_config};
    default:
        break;
    }
    return {};
}

QList<UpdateInfo> updates() noexcept
{
    return {update(HK_2_1_0_MAIN_CONFIG)};
}
} // namespace HKConfigUpdate
