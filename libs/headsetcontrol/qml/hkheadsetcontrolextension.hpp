// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETCONTROLEXTENSION_HPP
#define HKHEADSETCONTROLEXTENSION_HPP

#include "hkheadsetcontrol.hpp"

#include <QQmlListProperty>

class HKHC_NO_EXPORT HKHeadsetControlExtension : public QObject
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

#endif // HKHEADSETCONTROLEXTENSION_HPP
