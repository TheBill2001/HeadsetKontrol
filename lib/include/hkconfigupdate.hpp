// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKCONFIGUPDATE_HPP
#define HKCONFIGUPDATE_HPP

#include "headsetkontrol_export.hpp"

#include <QList>

namespace HKConfigUpdate
{
using UpdateFunc = int (*)();

enum UpdateId : quint8 {
    NONE = 0,
    HK_2_1_0_MAIN_CONFIG
};

struct HK_EXPORT UpdateInfo {
    UpdateId id{NONE};
    QString idString;
    UpdateFunc update{nullptr};

    [[nodiscard]] constexpr operator bool() const noexcept // NOLINT(*explicit-conversions)
    {
        return update != nullptr;
    }

    [[nodiscard]] int operator()() const
    {
        if (update != nullptr) {
            return update();
        }
        return EXIT_FAILURE;
    }
};

[[nodiscard]] HK_EXPORT UpdateInfo update(UpdateId updateId) noexcept;
[[nodiscard]] HK_EXPORT QList<UpdateInfo> updates() noexcept;
} // namespace HKConfigUpdate

#endif // HKCONFIGUPDATE_HPP
