// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKQMLUTILS_HPP
#define HKQMLUTILS_HPP

#include "hkutils.hpp"

#include <QMetaEnum>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class HK_NO_EXPORT HKQmlUtils : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(HKUtils)
    QML_SINGLETON
public:
    Q_INVOKABLE [[nodiscard]] static qint32 localeStringToMilliseconds(const QString &text, const QLocale &locale);
    Q_INVOKABLE [[nodiscard]] static QString millisecondsToLocaleString(int value);

    Q_INVOKABLE [[nodiscard]] Q_ALWAYS_INLINE static QString batteryStatusToLocaleString(const HKBattery &battery)
    {
        return HKUtils::batteryStatusToLocaleString(battery);
    }

    Q_INVOKABLE [[nodiscard]] Q_ALWAYS_INLINE static QString batteryIconName(const HKBattery &battery)
    {
        return HKUtils::batteryIconName(battery);
    }

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

    Q_INVOKABLE [[nodiscard]] static qsizetype countCapabilities(quint64 caps);

    Q_INVOKABLE [[nodiscard]] static QString capabilityLocaleString(quint64 cap);

    Q_INVOKABLE [[nodiscard]] static QStringList capabilityLocaleStringList(quint64 caps);
};

Q_MOC_INCLUDE(<QLocale>)
Q_MOC_INCLUDE("hkbattery.hpp")

#endif // HKQMLUTILS_HPP
