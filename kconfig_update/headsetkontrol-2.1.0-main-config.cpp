// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkconfigmigrate.hpp"

int main()
{
    return HKConfigUpdate::Migrate::update_2_1_0_main_config(KSharedConfig::openConfig(QLatin1StringView("headsetkontrolrc"), KConfig::SimpleConfig));
}
