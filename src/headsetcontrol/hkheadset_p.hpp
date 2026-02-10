// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSET_P_HPP
#define HKHEADSET_P_HPP

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkglobal.hpp"
#include "hkheadset.hpp"
#include "hkheadseterror.hpp"
#include "hkheadsetid.hpp"
#include "hkobjectprivate.hpp"

#include <headsetcontrol.hpp>

#include <QMutex>
#include <QObjectBindableProperty>

class HKHeadset::InitData
{
public:
    QMutex &internalMutex; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    headsetcontrol::Headset internalHeadset;
};

class HKHeadsetPrivate : public HKObjectPrivate<HKHeadset>
{
    Q_DISABLE_COPY(HKHeadsetPrivate)
    Q_DECLARE_PUBLIC(HKHeadset)
public:
    std::atomic<bool> updating{false};
    QMutex &internalMutex;
    mutable headsetcontrol::Headset internalHeadset;

    const HKHeadsetId id;
    const QString name;

    HK_PRIVATE_BINDABLE_TRIVIAL_PROPERTY(HKHeadset, HKHeadset::Capabilities, capabilities) // NOLINT(clazy-incorrect-emit)

    [[nodiscard]] qsizetype countCapabilities() const;
    Q_OBJECT_COMPUTED_PROPERTY(HKHeadsetPrivate, qsizetype, capabilityCount, &HKHeadsetPrivate::countCapabilities)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadset, QList<HKHeadsetError>, errors) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadset, HKBattery, battery) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadset, HKChatMix, chatMix) // NOLINT(clazy-incorrect-emit)

    HKHeadsetPrivate(HKHeadset::InitData &&initData, HKHeadset *const q_ptr);

    [[nodiscard]] HKHeadsetId getId() const;
    [[nodiscard]] QString getName() const;
    [[nodiscard]] HKHeadset::Capabilities getCapabilities() const;
    [[nodiscard]] HKHeadsetError makeError(const headsetcontrol::DeviceError &error, HKHeadset::Capability capability) const;

    template<typename F, typename T = HKFunctionReturnType<F>, typename A = HKFunctionArgumentType<0, F>, typename R = std::remove_cvref_t<A>>
    [[nodiscard]] T getResult(QList<HKHeadsetError> &errors, headsetcontrol::Result<R> &&result, HKHeadset::Capability capability, F &&create) const
    {
        if (result.hasValue()) {
            if constexpr (std::is_rvalue_reference_v<A>) {
                return std::invoke(std::forward<F>(create), std::move(result).value());
            } else {
                return std::invoke(std::forward<F>(create), result.value());
            }
        } else if (result.hasError()) {
            errors << makeError(result.error(), capability);
        } else {
            Q_UNLIKELY_BRANCH;
            HK_WARN("Result has neither value nor error!");
        }
        return T{};
    }

    [[nodiscard]] HKBattery getBattery(QList<HKHeadsetError> &errors) const;
    [[nodiscard]] HKChatMix getChatMix(QList<HKHeadsetError> &errors) const;

    [[maybe_unused]] bool refresh(bool block = false);
};

#endif // HKHEADSET_P_HPP
