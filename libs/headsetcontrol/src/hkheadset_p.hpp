// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSET_P_HPP
#define HKHEADSET_P_HPP

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkhclogging_p.hpp"
#include "hkhcutils_p.hpp"
#include "hkheadset.hpp"
#include "hkheadseterror.hpp"
#include "hkheadsetid.hpp"
#include "hkmacros_p.hpp"

#include <headsetcontrol.hpp>

#include <QMutex>
#include <QObjectBindableProperty>

#include <KLocalizedString>

class HKHeadset::InitData
{
public:
    QMutex &internalMutex; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    headsetcontrol::Headset internalHeadset;
};

class HKHeadsetPrivate
{
    Q_DISABLE_COPY(HKHeadsetPrivate)
    Q_DECLARE_PUBLIC(HKHeadset)
    HKHeadset *const q_ptr;

public:
    std::atomic<bool> updating{false};
    QMutex &internalMutex;
    mutable headsetcontrol::Headset internalHeadset;

    const HKHeadsetId id;
    const QString name;

    HK_PRIVATE_BINDABLE_TRIVIAL_PROPERTY(HKHeadset, HKHeadset::Capabilities, capabilities) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadset, QList<HKHeadsetError>, errors) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadset, HKBattery, battery) // NOLINT(clazy-incorrect-emit)

    HK_PRIVATE_BINDABLE_PROPERTY(HKHeadset, HKChatMix, chatMix) // NOLINT(clazy-incorrect-emit)

    HKHeadsetPrivate(HKHeadset::InitData &&initData, HKHeadset *const q_ptr);

    [[nodiscard]] HKHeadsetId getId() const;
    [[nodiscard]] QString getName() const;
    [[nodiscard]] HKHeadset::Capabilities getCapabilities() const;
    [[nodiscard]] HKHeadsetError makeError(const headsetcontrol::DeviceError &error, HKHeadset::Capability capability) const;

    template<typename F, typename T = HKUtils::function_ret_t<F>, typename R = std::remove_cvref_t<HKUtils::function_arg_t<0, F>>>
    [[nodiscard]] T getResult(QList<HKHeadsetError> &errors, headsetcontrol::Result<R> &&result, HKHeadset::Capability capability, F &&create) const
    {
        if (result.hasValue()) {
            if constexpr (std::is_same_v<HKUtils::function_arg_t<0, F>, R &&>) {
                return std::invoke(std::forward<F>(create), std::move(result).value());
            } else {
                return std::invoke(std::forward<F>(create), result.value());
            }
        } else if (result.hasError()) {
            errors << makeError(result.error(), capability);
        } else {
            Q_UNLIKELY_BRANCH;
            qCWarning(HKHC_LOGGING).noquote() << i18nc("@info:shell result from native library is invalid", "Result has neither value nor error!");
        }
        return T{};
    }

    [[nodiscard]] HKBattery getBattery(QList<HKHeadsetError> &errors) const;
    [[nodiscard]] HKChatMix getChatMix(QList<HKHeadsetError> &errors) const;

    void refresh();

    friend QBindingStorage *qGetBindingStorage(HKHeadsetPrivate *const object);
    friend const QBindingStorage *qGetBindingStorage(const HKHeadsetPrivate *const object);
};

[[nodiscard]] Q_ALWAYS_INLINE QBindingStorage *qGetBindingStorage(HKHeadsetPrivate *const object)
{
    return object->q_ptr->bindingStorage();
}

[[nodiscard]] Q_ALWAYS_INLINE const QBindingStorage *qGetBindingStorage(const HKHeadsetPrivate *const object)
{
    return static_cast<const HKHeadset *>(object->q_ptr)->bindingStorage();
}

#endif // HKHEADSET_P_HPP
