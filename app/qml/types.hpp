// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHC_TYPES_QML_HPP
#define HKHC_TYPES_QML_HPP

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkconfig.hpp"
#include "hkheadset.hpp"
#include "hkheadsetcontrol.hpp"
#include "hkheadseterror.hpp"
#include "hkheadsetid.hpp"

#include <QQmlEngine>
#include <QQmlListProperty>

class HK_NO_EXPORT HKConfigForeign
{
    Q_GADGET
    QML_SINGLETON
    QML_NAMED_ELEMENT(HKConfig)
    QML_FOREIGN(HKConfig)

    HKConfigForeign() = default;

public:
    [[nodiscard]] static HKConfig *create(QQmlEngine *engine, QJSEngine * /*unused*/);
};

struct HK_NO_EXPORT HKHeadsetIdForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkHeadsetId)
    QML_FOREIGN(HKHeadsetId)
    QML_STRUCTURED_VALUE
};

struct HK_NO_EXPORT HKHeadsetErrorForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(hkHeadsetError)
    QML_FOREIGN(HKHeadsetError)
};

class HK_NO_EXPORT HKHeadsetErrorDerived : public HKHeadsetError
{
    Q_GADGET
};

namespace HKHeadsetErrorDerivedForeign
{
Q_NAMESPACE_EXPORT(HK_NO_EXPORT)
QML_NAMED_ELEMENT(HKHeadsetError)
QML_FOREIGN_NAMESPACE(HKHeadsetErrorDerived)
}

struct HK_NO_EXPORT HKBatteryForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkBattery)
    QML_FOREIGN(HKBattery)
};

struct HK_NO_EXPORT HKChatMixForeign {
    Q_GADGET
    QML_VALUE_TYPE(hkChatMix)
    QML_FOREIGN(HKChatMix)
};

struct HK_NO_EXPORT HKHeadsetForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(HKHeadset)
    QML_UNCREATABLE("Managed by HKHeadsetControl")
    QML_FOREIGN(HKHeadset)
};

class HK_NO_EXPORT HKHeadsetControlExtension : public QObject
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

class HK_NO_EXPORT HKHeadsetControlForeign
{
    Q_GADGET
    QML_SINGLETON
    QML_NAMED_ELEMENT(HKHeadsetControl)
    QML_FOREIGN(HKHeadsetControl)
    QML_EXTENDED(HKHeadsetControlExtension)

    HKHeadsetControlForeign() = default;

public:
    [[nodiscard]] static HKHeadsetControl *create(QQmlEngine *engine, QJSEngine * /*unused*/);
};

#endif // HKHC_TYPES_QML_HPP
