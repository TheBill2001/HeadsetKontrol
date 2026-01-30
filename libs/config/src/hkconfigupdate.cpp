// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkconfigupdate.hpp"

#include "hkconfig.hpp"

#if !defined(USE_KCONFIG_UPDATE) || !defined(Q_OS_LINUX)
#include "hkconfig_logging_p.hpp"
#include "hkconfigmigrate.hpp"
#endif

using namespace Qt::StringLiterals;

namespace HKConfigUpdate
{
void migrate()
{
    const QString updateFile = "headsetkontrol.upd"_L1;

#if !defined(USE_KCONFIG_UPDATE) || !defined(Q_OS_LINUX)
// NOLINTNEXTLINE(*macro-usage)
#define DO_UPDATE(updateId, Func, ...)                                                                                                                         \
    {                                                                                                                                                          \
        QString fullId = updateFile + ':'_L1 + updateId;                                                                                                       \
        if (!ids.contains(fullId)) {                                                                                                                           \
            const auto ret = Func(__VA_ARGS__);                                                                                                                \
            if (ret == EXIT_SUCCESS) {                                                                                                                         \
                ids.append(std::move(fullId));                                                                                                                 \
            } else {                                                                                                                                           \
                qCCritical(HKCONFIG_LOGGING, "Failed to update config ID with exit code %d: %s", ret, qPrintable(updateId));                                   \
            }                                                                                                                                                  \
        }                                                                                                                                                      \
    }

    { // Migrate main config
        const auto config = HKConfig::self()->sharedConfig();
        auto versionGroup = config->group("$Version"_L1);
        QStringList ids = versionGroup.readEntry("update_info", QStringList{});

        DO_UPDATE("headsetkontrol-2.1.0-main-config"_L1, update_2_1_0_main_config, config)

        versionGroup.writeEntry("update_info", ids);
        HKConfig::self()->save();
    }
#else
    { // Migrate main config
        auto config = HKConfig::self()->sharedConfig();
        const QStringList updateIds{"headsetkontrol-2.1.0-main-config"_L1};
        for (const auto &updateId : updateIds) {
            config->checkUpdate(updateId, updateFile);
        }
    }
#endif
}
} // namespace HKConfigUpdate
