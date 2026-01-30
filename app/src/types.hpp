// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHC_QML_TYPES_HPP
#define HKHC_QML_TYPES_HPP

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkconfig.hpp"
#include "hkheadset.hpp"
#include "hkheadsetcontrolextension.hpp"
#include "hkheadseterror.hpp"
#include "hkheadsetid.hpp"

#include <QQmlEngine>

struct HKConfigForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKConfig)
    QML_FOREIGN(HKConfig)
    QML_SINGLETON
private:
    HKConfigForeign() = default;

public:
    [[nodiscard]] static HKConfig *create(QQmlEngine *engine, QJSEngine * /*unused*/);
};

struct HKBatteryForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkBattery)
    QML_FOREIGN(HKBattery)
};

struct HKBatteryDerived : public HKBattery {
    Q_GADGET
};

namespace HKBatteryDerivedForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(HKBattery)
QML_FOREIGN_NAMESPACE(HKBatteryDerived)
}

struct HKChatMixForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkChatMix)
    QML_FOREIGN(HKChatMix)
};

struct HKHeadsetIdForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkHeadsetId)
    QML_CONSTRUCTIBLE_VALUE
    QML_FOREIGN(HKHeadsetId)
};

struct HKHeadsetErrorForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkHeadsetError)
    QML_FOREIGN(HKHeadsetError)
};

struct HKHeadsetErrorDerived : public HKHeadsetError {
    Q_GADGET
};

namespace HKHeadsetErrorDerivedForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(HKHeadsetError)
QML_FOREIGN_NAMESPACE(HKHeadsetErrorDerived)
}

struct HKHeadsetForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKHeadset)
    QML_UNCREATABLE("Managed by HKHeadsetControl")
    QML_FOREIGN(HKHeadset)
};

struct HKHeadsetControlForeign {
    Q_GADGET
    QML_SINGLETON
    QML_NAMED_ELEMENT(HKHeadsetControl)
    QML_EXTENDED(HKHeadsetControlExtension)
    QML_FOREIGN(HKHeadsetControl)

private:
    HKHeadsetControlForeign();

public:
    [[nodiscard]] static HKHeadsetControl *create(QQmlEngine *engine, QJSEngine * /*unused*/);
};

#endif // HKHC_QML_TYPES_HPP
