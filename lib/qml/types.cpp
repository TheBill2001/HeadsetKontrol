// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "types.hpp"

HKConfig *HKConfigForeign::create(QQmlEngine *engine, QJSEngine * /*unused*/)
{
    auto *const instance = HKConfig::self();
    Q_ASSERT(instance->thread() == engine->thread());
    QQmlEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

HKHeadsetControlExtension::HKHeadsetControlExtension(QObject *parent)
    : QObject{parent}
    , m_parent{qobject_cast<HKHeadsetControl *>(parent)}
{
    Q_ASSERT(m_parent != nullptr);

    connect(m_parent, &HKHeadsetControl::headsetsChanged, this, [this](const QList<HKHeadset *> &value) {
        m_headsets = value;
        Q_EMIT headsetsChanged(QPrivateSignal{});
    });
}

QQmlListProperty<HKHeadset> HKHeadsetControlExtension::headsets()
{
    return {this,
            &m_headsets,
            [](QQmlListProperty<HKHeadset> *property) {
                return static_cast<QList<HKHeadset *> *>(property->data)->size();
            },
            [](QQmlListProperty<HKHeadset> *property, qsizetype index) {
                return static_cast<QList<HKHeadset *> *>(property->data)->at(index);
            }};
};

HKHeadsetControl *HKHeadsetControlForeign::create(QQmlEngine *engine, QJSEngine * /*unused*/)
{
    auto *const instance = HKHeadsetControl::instance();
    Q_ASSERT(instance->thread() == engine->thread());
    QQmlEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}
