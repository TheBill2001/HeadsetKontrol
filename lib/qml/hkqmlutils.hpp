// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKQMLUTILS_HPP
#define HKQMLUTILS_HPP

#include "headsetkontrol_export.hpp"

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

class HK_NO_EXPORT HKQmlUtils : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(HKUtils)
    QML_SINGLETON
public:
    Q_INVOKABLE static int localeStringToMilliseconds(const QString &text, const QLocale &locale);
    Q_INVOKABLE static QString millisecondsToLocaleString(int value);
};

Q_MOC_INCLUDE(<QLocale>)

#endif // HKQMLUTILS_HPP
