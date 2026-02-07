// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef TYPES_HPP
#define TYPES_HPP

#include "hkconfig.hpp"
#include "hklogstorage.hpp"

#include <QQmlEngine>

struct HKAPP_NO_EXPORT HKLogEntryForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkLogEntry)
    QML_FOREIGN(HKLogEntry)
};

struct HKAPP_NO_EXPORT HKLogStorageForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKLogStorage)
    QML_SINGLETON
    QML_FOREIGN(HKLogStorage)
public:
    [[nodiscard]] static HKLogStorage *create(QQmlEngine *, QJSEngine *);

private:
    HKLogStorageForeign() = default;
};

struct HKAPP_NO_EXPORT HKConfigForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKConfig)
    QML_SINGLETON
    QML_FOREIGN(HKConfig)
public:
    [[nodiscard]] static HKConfig *create(QQmlEngine *, QJSEngine *);

private:
    HKConfigForeign() = default;
};

#endif // TYPES_HPP
