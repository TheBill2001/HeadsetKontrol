// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKCONFIGMIGRATE_HPP
#define HKCONFIGMIGRATE_HPP

#include "hkconfig_export.hpp"

#include <KSharedConfig>

namespace HKConfigUpdate::Migrate
{
int HKCONFIG_EXPORT update_2_1_0_main_config(KSharedConfig::Ptr config);
} // namespace HKConfigUpdate::Migrate

#endif // HKCONFIGMIGRATE_HPP
