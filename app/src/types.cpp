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

HKHeadsetControl *HKHeadsetControlForeign::create(QQmlEngine *engine, QJSEngine * /*unused*/)
{
    auto *const instance = HKHeadsetControl::instance();
    Q_ASSERT(instance->thread() == engine->thread());
    QQmlEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}
