// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "headsetkontrol_export.hpp"

#include "hkheadsetid.hpp"
#include "hklogging.hpp"

#include <QDebug>

#include <KConfig>
#include <KConfigGroup>

using namespace Qt::StringLiterals;

namespace HKConfigUpdate
{
int HK_NO_EXPORT update_2_1_0_main_config()
{
    KConfig config(u"headsetkontrolrc"_s);

    auto generalGroup = config.group(u"General"_s);
    auto userInterfaceGroup = config.group(u"UserInterface"_s);
    auto deviceGroup = config.group(u"Device"_s);
    auto headsetControlGroup = config.group(u"HeadsetControl"_s);

    if (generalGroup.hasKey(u"Primary"_s)) {
        const auto list = generalGroup.readEntry(u"Primary"_s, QStringList{});

        bool check = false;
        if (list.size() < 2) {
            HKHeadsetId headsetId{list[0], list[1]};
            if (headsetId.isValid()) {
                deviceGroup.writeEntry(u"primaryDevice"_s, headsetId.toString());
                deviceGroup.sync();
                check = true;
            }
        }

        if (!check) {
            qCWarning(HK_LOGGING) << "Invalid \"Primary\" entry:" << list;
        }

        generalGroup.deleteEntry(u"Primary"_s);
        generalGroup.sync();
    }

    if (userInterfaceGroup.hasKey(u"ShowCountdownProgress"_s)) {
        if (userInterfaceGroup.readEntry(u"ShowCountdownProgress"_s, false)) {
            userInterfaceGroup.writeEntry(u"showRefreshTimer"_s, true);
        }

        userInterfaceGroup.deleteEntry(u"ShowCountdownProgress"_s);
        userInterfaceGroup.sync();
    }

    headsetControlGroup.deleteEntry(u"ExecutablePath"_s);
    headsetControlGroup.sync();

    config.sync();
    return EXIT_SUCCESS;
}
} // namespace HKConfigUpdate
