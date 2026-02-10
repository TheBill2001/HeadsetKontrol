// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKUTILS_QML_HPP
#define HKUTILS_QML_HPP

#include "hkbattery.hpp"
#include "hkheadset.hpp"
#include "hkheadseterror.hpp"

#include <QLocale>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class QWindow;

class HKUtilsQml : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(HKUtils)
    QML_SINGLETON
public:
    Q_INVOKABLE [[nodiscard]] static qint32 localeStringToMilliseconds(const QString &text, const QLocale &locale);
    Q_INVOKABLE [[nodiscard]] static QString millisecondsToLocaleString(int value);

    Q_INVOKABLE [[nodiscard]] static QString batteryIconName(const HKBattery &battery, bool styled = false, const QString &fallback = {})
    {
        return HKUtils::batteryIconName(battery, styled, fallback);
    }

    Q_INVOKABLE [[nodiscard]] static QString batteryStatusToLocaleString(HKBattery::BatteryStatus status)
    {
        return HKUtils::batteryStatusToLocaleString(status);
    }

    Q_INVOKABLE [[nodiscard]] static QString capabilityToLocaleString(HKHeadset::Capability capability)
    {
        return HKUtils::capabilityToLocaleString(capability);
    }

    Q_INVOKABLE [[nodiscard]] static QStringList capabilitiesToLocaleStrings(HKHeadset::Capabilities capabilities)
    {
        return HKUtils::capabilitiesToLocaleStrings(capabilities);
    }

    Q_INVOKABLE [[nodiscard]] static QString headsetErrorLocaleErrorString(const HKHeadsetError &error)
    {
        return HKUtils::headsetErrorLocaleErrorString(error);
    }

    Q_INVOKABLE [[nodiscard]] static QString headsetErrorToLocaleString(HKHeadsetError::Error error)
    {
        return HKUtils::headsetErrorToLocaleString(error);
    }
};

Q_MOC_INCLUDE(<QLocale>)

#endif // HKUTILS_QML_HPP
