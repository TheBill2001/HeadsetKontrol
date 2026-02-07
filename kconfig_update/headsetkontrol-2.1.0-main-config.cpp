// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkconfupd_logging.hpp"

#include <KConfigGroup>
#include <KSharedConfig>

using namespace Qt::StringLiterals;

namespace
{
quint16 parseId(QAnyStringView idStringView)
{
    if (idStringView.isEmpty()) {
        return 0;
    }

    bool check = false;
    const auto idString = idStringView.toString();
    auto value = idString.simplified().toLower().toUInt(&check, 16);

    if (!check) {
        qWarning(HKCONFUPD_LOGGING, "Failed to parse device ID string: \"%s\"", qPrintable(idString));
        return 0;
    }

    if (value == 0 || value > std::numeric_limits<quint16>::max()) {
        qWarning(HKCONFUPD_LOGGING, "Parsed ID string is out of range: \"%s\"", qPrintable(idString));
        return 0;
    }

    return value;
}
}

int main()
{
    const auto config = KSharedConfig::openConfig(QLatin1StringView("headsetkontrolrc"), KConfig::SimpleConfig);

    auto generalGroup = config->group(u"General"_s);
    auto userInterfaceGroup = config->group(u"UserInterface"_s);
    auto deviceGroup = config->group(u"Device"_s);
    auto headsetControlGroup = config->group(u"HeadsetControl"_s);

    if (generalGroup.hasKey(u"Primary"_s)) {
        const auto list = generalGroup.readEntry(u"Primary"_s, QStringList{});

        bool check = false;
        if (list.size() < 2) {
            auto vendorId = parseId(list[0]);
            auto productId = parseId(list[1]);
            if (vendorId != 0 && productId != 0) {
                deviceGroup.writeEntry(u"primaryDevice"_s, u"0x%1:0x%2"_s.arg(QString::number(vendorId, 16), QString::number(productId, 16)));
                check = true;
            }
        }

        if (check) {
            generalGroup.deleteEntry(u"Primary"_s);
        } else {
            qCWarning(HKCONFUPD_LOGGING) << "Invalid \"Primary\" entry:" << list;
        }
    }

    if (userInterfaceGroup.hasKey(u"ShowCountdownProgress"_s)) {
        if (userInterfaceGroup.readEntry(u"ShowCountdownProgress"_s, false)) {
            userInterfaceGroup.writeEntry(u"showRefreshTimer"_s, true);
        }

        userInterfaceGroup.deleteEntry(u"ShowCountdownProgress"_s);
    }

    headsetControlGroup.deleteEntry(u"ExecutablePath"_s);

    return config->sync() ? EXIT_SUCCESS : EXIT_FAILURE;
}
