// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadset.hpp"

#include "hkheadset_p.hpp"
#include "hkutils.hpp"

#include <QCollator>
#include <QThreadPool>

namespace
{
constexpr HKHeadset::Capability capabilityFromInternal(::capabilities value) noexcept
{
    switch (value) {
    case CAP_SIDETONE:
        return HKHeadset::SidetoneCapability;
    case CAP_BATTERY_STATUS:
        return HKHeadset::BatteryStatusCapability;
    case CAP_NOTIFICATION_SOUND:
        return HKHeadset::NotificationSoundCapability;
    case CAP_LIGHTS:
        return HKHeadset::LightsCapability;
    case CAP_INACTIVE_TIME:
        return HKHeadset::InactiveTimeCapability;
    case CAP_CHATMIX_STATUS:
        return HKHeadset::ChatMixStatusCapability;
    case CAP_VOICE_PROMPTS:
        return HKHeadset::VoicePromptsCapability;
    case CAP_ROTATE_TO_MUTE:
        return HKHeadset::RotateToMuteCapability;
    case CAP_EQUALIZER_PRESET:
        return HKHeadset::EqualizerPresetCapability;
    case CAP_EQUALIZER:
        return HKHeadset::EqualizerCapability;
    case CAP_PARAMETRIC_EQUALIZER:
        return HKHeadset::ParametricEqualizerCapability;
    case CAP_MICROPHONE_MUTE_LED_BRIGHTNESS:
        return HKHeadset::MicrophoneMuteLedBrightnessCapability;
    case CAP_MICROPHONE_VOLUME:
        return HKHeadset::MicrophoneVolumeCapability;
    case CAP_VOLUME_LIMITER:
        return HKHeadset::VolumeLimiterCapability;
    case CAP_BT_WHEN_POWERED_ON:
        return HKHeadset::BluetoothWhenPoweredOnCapability;
    case CAP_BT_CALL_VOLUME:
        return HKHeadset::BluetoothCallVolumeCapability;
    default:
        break;
    }
    return HKHeadset::UnknownCapability;
}

constexpr HKHeadset::Capabilities capabilitiesFromInternal(int mask) noexcept
{
    HKHeadset::Capabilities flags;
    for (int i = 0; i < NUM_CAPABILITIES; ++i) {
        if (has_capability(mask, static_cast<::capabilities>(i))) {
            flags.setFlag(capabilityFromInternal(static_cast<::capabilities>(i)));
        }
    }
    return flags;
}

[[nodiscard]] constexpr HKHeadsetError::Error headsetErrorCodeFromInternal(headsetcontrol::DeviceError::Code value) noexcept
{
    switch (value) {
    case headsetcontrol::DeviceError::Code::Timeout:
        return HKHeadsetError::Timeout;
    case headsetcontrol::DeviceError::Code::DeviceOffline:
        return HKHeadsetError::DeviceOffline;
    case headsetcontrol::DeviceError::Code::ProtocolError:
        return HKHeadsetError::ProtocolError;
    case headsetcontrol::DeviceError::Code::InvalidParameter:
        return HKHeadsetError::InvalidParameter;
    case headsetcontrol::DeviceError::Code::NotSupported:
        return HKHeadsetError::NotSupported;
    case headsetcontrol::DeviceError::Code::PermissionDenied:
        return HKHeadsetError::PermissionDenied;
    case headsetcontrol::DeviceError::Code::USBError:
        return HKHeadsetError::UsbError;
    case headsetcontrol::DeviceError::Code::HIDError:
        return HKHeadsetError::HidError;
    case headsetcontrol::DeviceError::Code::OutOfBounds:
        return HKHeadsetError::OutOfBounds;
    default:
        break;
    }
    return HKHeadsetError::Unknown;
}

[[nodiscard]] constexpr HKBattery::BatteryStatus batteryStatusFromInternal(::battery_status value) noexcept
{
    switch (value) {
    case BATTERY_CHARGING:
        return HKBattery::BatteryCharging;
    case BATTERY_AVAILABLE:
        return HKBattery::BatteryAvailable;
    case BATTERY_HIDERROR:
        return HKBattery::BatteryHidError;
    case BATTERY_TIMEOUT:
        return HKBattery::BatteryTimeout;
    default:
        break;
    }
    return HKBattery::BatteryUnavailable;
}

[[nodiscard]] constexpr HKBattery::MicrophoneStatus microphoneStatusFromInternal(::microphone_status value) noexcept
{
    switch (value) {
    case MICROPHONE_UP:
        return HKBattery::MicrophoneUp;
    default:
        break;
    }
    return HKBattery::MicrophoneUnknown;
}

struct Data {
    HKHeadset::Capabilities capabilities;
    HKBattery battery;
    HKChatMix chatMix;
};
}

HKHeadsetPrivate::HKHeadsetPrivate(HKHeadset::InitData &&initData, HKHeadset *const q_ptr)
    : HKObjectPrivate{q_ptr}
    , internalMutex{std::move(initData).internalMutex}
    , internalHeadset{std::move(initData).internalHeadset}
    , id{getId()}
    , name{getName()}
    , capabilities{getCapabilities()}
{
}

qsizetype HKHeadsetPrivate::countCapabilities() const
{
    return HKUtils::countFlags(capabilities.value());
}

QStringList HKHeadsetPrivate::getCapabilitiesLocaleStrings() const
{
    return HKHeadset::capabilitiesToLocaleStrings(capabilities.value());
}

HKHeadsetId HKHeadsetPrivate::getId() const
{
    QMutexLocker lock(&internalMutex);
    return std::make_pair(internalHeadset.vendorId(), internalHeadset.productId());
}

QString HKHeadsetPrivate::getName() const
{
    QMutexLocker lock(&internalMutex);
    return STD_STRVIEW_TO_QSTR(internalHeadset.name());
}

HKHeadset::Capabilities HKHeadsetPrivate::getCapabilities() const
{
    QMutexLocker lock(&internalMutex);
    return capabilitiesFromInternal(internalHeadset.capabilitiesMask());
}

HKHeadsetError HKHeadsetPrivate::makeError(const headsetcontrol::DeviceError &error, HKHeadset::Capability capability) const
{
    return {.headset = static_cast<HKHeadset *>(q_ptr), // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            .headsetId = id,
            .headsetName = name,
            .capability = capability,
            .error = headsetErrorCodeFromInternal(error.code),
            .errorString = STD_STRVIEW_TO_QSTR(error.message),
            .details = STD_STRVIEW_TO_QSTR(error.details),
            .timestamp = QDateTime::fromStdTimePoint(std::chrono::time_point_cast<std::chrono::milliseconds>(error.timestamp))};
}

HKBattery HKHeadsetPrivate::getBattery(QList<HKHeadsetError> &errors) const
{
    return getResult(errors, internalHeadset.getBattery(), HKHeadset::BatteryStatusCapability, [](headsetcontrol::BatteryResult &&result) -> HKBattery {
        return {.level = HKUtils::clamp<quint8, 0, 100>(result.level_percent),
                .status = batteryStatusFromInternal(result.status),
                .microphoneStatus = microphoneStatusFromInternal(result.mic_status),
                .optionalVoltage = std::move(result).voltage_mv,
                .optionalTimeToEmpty = std::move(result).time_to_empty_min,
                .optionalTimeToFull = std::move(result).time_to_full_min};
    });
}

HKChatMix HKHeadsetPrivate::getChatMix(QList<HKHeadsetError> &errors) const
{
    return getResult(errors, internalHeadset.getChatmix(), HKHeadset::ChatMixStatusCapability, [](const headsetcontrol::ChatmixResult &result) -> HKChatMix {
        return {.level = HKUtils::clamp<quint8, 0, 128>(result.level),
                .gameVolumePercent = HKUtils::clamp<quint8, 0, 100>(result.game_volume_percent),
                .chatVolumePercent = HKUtils::clamp<quint8, 0, 100>(result.chat_volume_percent)};
    });
}

void HKHeadsetPrivate::refresh(bool block)
{
    QList<HKHeadsetError> errors;
    Data data{.capabilities = getCapabilities(), .battery = getBattery(errors), .chatMix = getChatMix(errors)};

    auto connectionType = Qt::AutoConnection;
    if (QThread::currentThread() != q_ptr->thread()) {
        connectionType = block ? Qt::BlockingQueuedConnection : Qt::AutoConnection;
    }

    q_ptr->metaObject()->invokeMethod(
        q_ptr,
        [this, data, errors = std::move(errors)]() mutable {
            capabilities = std::move(data).capabilities;
            battery = std::move(data).battery;
            chatMix = std::move(data).chatMix;

            if (!errors.isEmpty()) {
                Q_EMIT q_func()->refreshErrorOccurred(errors, HKHeadset::QPrivateSignal{});
            }
            this->errors = std::move(errors);

            Q_EMIT q_func()->refreshDone(HKHeadset::QPrivateSignal{});
        },
        connectionType);
}

HKHeadset::HKHeadset(InitData &&initData)
    : d_ptr{new HKHeadsetPrivate(std::move(initData), this)}
{
}

HKHeadset::~HKHeadset()
{
    delete d_ptr;
}

HKHeadsetId HKHeadset::id() const noexcept
{
    Q_D(const HKHeadset);
    return d->id;
}

QString HKHeadset::name() const noexcept
{
    Q_D(const HKHeadset);
    return d->name;
}

QBindable<HKHeadset::Capabilities> HKHeadset::bindableCapabilities() const
{
    Q_D(const HKHeadset);
    return &d->capabilities;
}

HKHeadset::Capabilities HKHeadset::capabilities() const
{
    Q_D(const HKHeadset);
    return d->capabilities.value();
}

QBindable<qsizetype> HKHeadset::bindableCapabilityCount() const
{
    Q_D(const HKHeadset);
    return &d->capabilityCount;
}

qsizetype HKHeadset::capabilityCount() const
{
    Q_D(const HKHeadset);
    return d->capabilityCount.value();
}

QBindable<QStringList> HKHeadset::bindableCapabilitiesLocaleStrings() const
{
    Q_D(const HKHeadset);
    return &d->capabilitiesLocaleStrings;
}

QStringList HKHeadset::capabilitiesLocaleStrings() const
{
    Q_D(const HKHeadset);
    return d->capabilitiesLocaleStrings.value();
}

QBindable<QList<HKHeadsetError>> HKHeadset::bindableErrors() const
{
    Q_D(const HKHeadset);
    return &d->errors;
}

QList<HKHeadsetError> HKHeadset::errors() const
{
    Q_D(const HKHeadset);
    return d->errors.value();
}

QBindable<HKBattery> HKHeadset::bindableBattery() const
{
    Q_D(const HKHeadset);
    return &d->battery;
}

HKBattery HKHeadset::battery() const
{
    Q_D(const HKHeadset);
    return d->battery.value();
}

QBindable<HKChatMix> HKHeadset::bindableChatMix() const
{
    Q_D(const HKHeadset);
    return &d->chatMix;
}

HKChatMix HKHeadset::chatMix() const
{
    Q_D(const HKHeadset);
    return d->chatMix.value();
}

QString HKHeadset::capabilityToLocaleString(Capability capability)
{
    switch (capability) {
    case HKHeadset::SidetoneCapability:
        return i18nc("@item:intext headset capability", "Sidetone");
    case HKHeadset::BatteryStatusCapability:
        return i18nc("@item:intext headset capability", "Battery");
    case HKHeadset::NotificationSoundCapability:
        return i18nc("@item:intext headset capability", "Notification sounds");
    case HKHeadset::LightsCapability:
        return i18nc("@item:intext headset capability", "Lights");
    case HKHeadset::InactiveTimeCapability:
        return i18nc("@item:intext headset capability", "Inactive time");
    case HKHeadset::ChatMixStatusCapability:
        return i18nc("@item:intext headset capability", "ChatMix");
    case HKHeadset::VoicePromptsCapability:
        return i18nc("@item:intext headset capability", "Voice prompts");
    case HKHeadset::RotateToMuteCapability:
        return i18nc("@item:intext headset capability", "Rotate to mute");
    case HKHeadset::EqualizerPresetCapability:
        return i18nc("@item:intext headset capability", "Equalizer presets");
    case HKHeadset::EqualizerCapability:
        return i18nc("@item:intext headset capability", "Equalizer");
    case HKHeadset::ParametricEqualizerCapability:
        return i18nc("@item:intext headset capability", "Parametric equalizer");
    case HKHeadset::MicrophoneMuteLedBrightnessCapability:
        return i18nc("@item:intext headset capability", "Microphone mute led brightness");
    case HKHeadset::MicrophoneVolumeCapability:
        return i18nc("@item:intext headset capability", "Microphone volume");
    case HKHeadset::VolumeLimiterCapability:
        return i18nc("@item:intext headset capability", "Volume limiter");
    case HKHeadset::BluetoothWhenPoweredOnCapability:
        return i18nc("@item:intext headset capability", "Bluetooth when powered on");
    case HKHeadset::BluetoothCallVolumeCapability:
        return i18nc("@item:intext headset capability", "Bluetooth call volume");
    default:
        break;
    }
    return i18nc("@item:intext headset capability", "Unkown capability");
}

QStringList HKHeadset::capabilitiesToLocaleStrings(Capabilities capabilities)
{
    QStringList stringList;
    const auto metaEnum = QMetaEnum::fromType<Capabilities>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        const auto value = Capability(metaEnum.value64(i).value_or(0));
        if (capabilities.testFlag(value)) {
            stringList << capabilityToLocaleString(value);
        }
    }
    QCollator collator;
    std::ranges::sort(stringList, collator);
    return stringList;
}

void HKHeadset::refresh()
{
    QThreadPool::globalInstance()->start([this] {
        Q_D(HKHeadset);
        d->refresh();
    });
}
