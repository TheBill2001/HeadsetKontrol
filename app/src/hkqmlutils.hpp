// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKQMLUTILS_HPP
#define HKQMLUTILS_HPP

#include "hkbattery.hpp"
#include "hkheadset.hpp"
#include "hkheadseterror.hpp"

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

    Q_INVOKABLE [[nodiscard]] Q_ALWAYS_INLINE static QString batteryStatusToLocaleString(HKBattery::BatteryStatus status)
    {
        return HKBattery::batteryStatusToLocaleString(status);
    }

    Q_INVOKABLE [[nodiscard]] Q_ALWAYS_INLINE static QString capabilityToLocaleString(HKHeadset::Capability capability)
    {
        return HKHeadset::capabilityToLocaleString(capability);
    }

    Q_INVOKABLE [[nodiscard]] Q_ALWAYS_INLINE static QStringList capabilitiesToLocaleStrings(HKHeadset::Capabilities capabilities)
    {
        return HKHeadset::capabilitiesToLocaleStrings(capabilities);
    }

    Q_INVOKABLE [[nodiscard]] Q_ALWAYS_INLINE static QString headsetErrorToLocaleString(HKHeadsetError::Error error)
    {
        return HKHeadsetError::errorToLocaleString(error);
    }
};

Q_MOC_INCLUDE(<QLocale>)
Q_MOC_INCLUDE("hkbattery.hpp")

#endif // HKQMLUTILS_HPP
