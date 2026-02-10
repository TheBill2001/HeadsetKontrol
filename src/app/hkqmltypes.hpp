// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKQMLTYPES_HPP
#define HKQMLTYPES_HPP

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkconfig.hpp"
#include "hkheadset.hpp"
#include "hkheadsetcontrol.hpp"
#include "hkheadsetid.hpp"
#include "hklogstorage.hpp"

#include <QQmlEngine>
#include <QQmlListProperty>

struct HKBatteryForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkBattery)
    QML_FOREIGN(HKBattery)
};

class HKBatteryDerived : public HKBattery
{
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
    QML_FOREIGN(HKHeadsetId)
    QML_CONSTRUCTIBLE_VALUE
};

struct HKHeadsetForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKHeadset)
    QML_UNCREATABLE("Managed by HKHeadsetControl")
    QML_FOREIGN(HKHeadset)
};

class HKHeadsetControlExtension : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<HKHeadset> headsets READ headsets NOTIFY headsetsChanged FINAL)

    HKHeadsetControl *const m_parent;
    QList<HKHeadset *> m_headsets;

public:
    explicit HKHeadsetControlExtension(QObject *parent);

    [[nodiscard]] QQmlListProperty<HKHeadset> headsets();

Q_SIGNALS:
    void headsetsChanged(QPrivateSignal);
};

struct HKHeadsetControlForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKHeadsetControl)
    QML_SINGLETON
    QML_EXTENDED(HKHeadsetControlExtension)
    QML_FOREIGN(HKHeadsetControl)
public:
    [[nodiscard]] static HKHeadsetControl *create(QQmlEngine *engine, QJSEngine *);

private:
    HKHeadsetControlForeign() = default;
};

struct HKLogStorageForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKLogStorage)
    QML_SINGLETON
    QML_FOREIGN(HKLogStorage)
public:
    [[nodiscard]] static HKLogStorage *create(QQmlEngine *engine, QJSEngine *);

private:
    HKLogStorageForeign() = default;
};

#endif // HKQMLTYPES_HPP
