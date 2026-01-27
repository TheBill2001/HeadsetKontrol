// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkqmlutils.hpp"

#include "hkheadset.hpp"

#include <QCollator>
#include <QLocale>

#include <KLocalizedString>

using namespace Qt::StringLiterals;

qint32 HKQmlUtils::localeStringToMilliseconds(const QString &text, const QLocale &locale)
{
    const auto _text =
        QString(text).replace(i18nc("@item:valuesuffix", "seconds"), QString{}).replace(i18nc("@item:valuesuffix", "second"), QString{}).trimmed();
    bool check = false;
    const auto value = locale.toInt(_text, &check);
    if (check) {
        return value;
    }
    return 0;
}

QString HKQmlUtils::millisecondsToLocaleString(int value)
{
    return i18ncp("@item:valuesuffix", "%1 millisecond", "%1 milliseconds", value);
}
