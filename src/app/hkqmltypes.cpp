// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkqmltypes.hpp"

namespace
{
template<typename T>
T *create(QQmlEngine *engine, T *instance)
{
    Q_ASSERT(instance->thread() == engine->thread());
    QQmlEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

template<typename T>
T *create(QQmlEngine *engine)
{
    return create(engine, T::instance());
}
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

    m_headsets = m_parent->headsets();
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
    return ::create<HKHeadsetControl>(engine);
}

HKLogStorage *HKLogStorageForeign::create(QQmlEngine *engine, QJSEngine * /*unused*/)
{
    return ::create<HKLogStorage>(engine);
}
