// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkconfigupdate.hpp"

int main()
{
    return HKConfigUpdate::update(HKConfigUpdate::HK_2_1_0_MAIN_CONFIG)();
}
