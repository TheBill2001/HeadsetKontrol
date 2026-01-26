// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHC_TYPES_QML_HPP
#define HKHC_TYPES_QML_HPP

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkheadset.hpp"
#include "hkheadsetcontrol.hpp"
#include "hkheadseterror.hpp"

#include <QQmlEngine>
#include <QQmlListProperty>

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

class HK_NO_EXPORT HKBatteryDerived : public HKBattery
{
    Q_GADGET
};

namespace HKBatteryForeign
{
Q_NAMESPACE_EXPORT(HK_NO_EXPORT)
QML_NAMED_ELEMENT(HKBattery)
QML_FOREIGN_NAMESPACE(HKBatteryDerived)
}

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

#endif // HKHC_TYPES_QML_HPP
