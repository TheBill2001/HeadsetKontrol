// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKUTILS_HPP
#define HKUTILS_HPP

#include "hkbattery.hpp"
#include "hkheadset.hpp"
#include "hkheadseterror.hpp"

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class HKUtils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    Q_INVOKABLE [[nodiscard]] static qint32 localeStringToMilliseconds(const QString &text, const QLocale &locale);
    Q_INVOKABLE [[nodiscard]] static QString millisecondsToLocaleString(int value);

    Q_INVOKABLE [[nodiscard]] static QString batteryIconName(const HKBattery &battery, bool styled = false, const QString &fallback = {});
    Q_INVOKABLE [[nodiscard]] static QString batteryStatusToLocaleString(HKBattery::BatteryStatus status);

    Q_INVOKABLE [[nodiscard]] static QString capabilityToLocaleString(HKHeadset::Capability capability);
    Q_INVOKABLE [[nodiscard]] static QStringList capabilitiesToLocaleStrings(HKHeadset::Capabilities capabilities);

    Q_INVOKABLE [[nodiscard]] static QString headsetErrorLocaleErrorString(const HKHeadsetError &error);
    Q_INVOKABLE [[nodiscard]] static QString headsetErrorToLocaleString(HKHeadsetError::Error error);
};

Q_MOC_INCLUDE(<QLocale>)

#endif // HKUTILS_HPP
