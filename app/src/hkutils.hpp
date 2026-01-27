// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKUTILS_HPP
#define HKUTILS_HPP

#include <QMetaEnum>

namespace HKUtils
{
template<typename Enum>
[[nodiscard]] static qsizetype countFlags(QFlags<Enum> flags)
{
    qsizetype count = 0;
    const auto metaEnum = QMetaEnum::fromType<QFlags<Enum>>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        if (flags.testFlag(Enum(metaEnum.value(i)))) {
            ++count;
        }
    }
    return count;
}
} // namespace HKUtils

#endif // HKUTILS_HPP
