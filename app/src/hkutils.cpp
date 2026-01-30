// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkutils.hpp"

#include <QCollator>
#include <QLocale>
#include <QMetaEnum>

#include <KLazyLocalizedString>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto timeErrorString = kli18nc("headset error string", "Operation timed out");
constexpr auto deviceOfflineErrorString = kli18nc("headset error string", "Device is offline or not responding");
constexpr auto protocolErrorString = kli18nc("headset error string", "Protocol error");
constexpr auto invalidParameterErrorString = kli18nc("headset error string", "Invalid parameter");
constexpr auto notSupportedErrorString = kli18nc("headset error string", "Feature not supported by this device");
constexpr auto permissionDeniedErrorString = kli18nc("headset error string", "Permission denied");
constexpr auto usbErrorString = kli18nc("headset error string", "USB communication error");
constexpr auto hidErrorString = kli18nc("headset error string", "HID communication error");
constexpr auto outOfBoundsErrorString = kli18nc("headset error string", "Out of bounds");
constexpr auto unknownErrorString = kli18nc("headset error string", "Unknown error");
}

qint32 HKUtils::localeStringToMilliseconds(const QString &text, const QLocale &locale)
{
    const auto _text =
        QString(text).replace(i18nc("@item:valuesuffix", "seconds"), QString{}).replace(i18nc("@item:valuesuffix", "second"), QString{}).trimmed();
    bool check = false;
    const auto value = locale.toInt(_text, &check);
    if (check) {
        return value;
    }
    return 0;
}

QString HKUtils::millisecondsToLocaleString(int value)
{
    return i18ncp("@item:valuesuffix", "%1 millisecond", "%1 milliseconds", value);
}

QString HKUtils::batteryIconName(const HKBattery &battery, bool styled, const QString &fallback)
{
    if (battery.status() > HKBattery::BatteryUnavailable) {
        const auto level = 10 * ((battery.level() + 5) / 10);
        QString iconName = u"battery-%1"_s.arg(level, 3, 10, '0'_L1);
        if (styled) {
            iconName = u"headsetkontrol-"_s + iconName;
        }
        if (battery.status() == HKBattery::BatteryCharging) {
            iconName.append(u"-charging"_s);
        }
        return iconName;
    }
    return fallback;
}

QString HKUtils::batteryStatusToLocaleString(HKBattery::BatteryStatus status)
{
    switch (status) {
    case HKBattery::BatteryHidError:
        return i18nc("@item:intext battery status", "HID error");
    case HKBattery::BatteryTimeout:
        return i18nc("@item:intext battery status", "Timed out");
    case HKBattery::BatteryCharging:
        return i18nc("@item:intext battery status", "Charging");
    case HKBattery::BatteryAvailable:
        return i18nc("@item:intext battery status", "Available");
    case HKBattery::BatteryUnavailable:
        break;
    }
    return i18nc("@item:intext battery status", "Unavailable");
}

QString HKUtils::capabilityToLocaleString(HKHeadset::Capability capability)
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

QStringList HKUtils::capabilitiesToLocaleStrings(HKHeadset::Capabilities capabilities)
{
    QStringList stringList;
    const auto metaEnum = QMetaEnum::fromType<HKHeadset::Capabilities>();
    if constexpr (sizeof(HKHeadset::Capability) > sizeof(decltype(metaEnum.value(0)))) {
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            const auto valueOpt = metaEnum.value64(i);
            if (valueOpt.has_value()) {
                const auto value = HKHeadset::Capability(valueOpt.value_or(0));
                if (capabilities.testFlag(value)) {
                    stringList << capabilityToLocaleString(value);
                }
            }
        }
    } else {
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            const auto value = HKHeadset::Capability(metaEnum.value(i));
            if (capabilities.testFlag(value)) {
                stringList << capabilityToLocaleString(value);
            }
        }
    }
    QCollator collator;
    std::ranges::sort(stringList, collator);
    return stringList;
}

QString HKUtils::headsetErrorLocaleErrorString(const HKHeadsetError &error)
{
    if (error.errorString() == QAnyStringView(timeErrorString.untranslatedText())) {
        return timeErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(deviceOfflineErrorString.untranslatedText())) {
        return deviceOfflineErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(protocolErrorString.untranslatedText())) {
        return protocolErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(invalidParameterErrorString.untranslatedText())) {
        return invalidParameterErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(notSupportedErrorString.untranslatedText())) {
        return notSupportedErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(permissionDeniedErrorString.untranslatedText())) {
        return permissionDeniedErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(usbErrorString.untranslatedText())) {
        return usbErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(hidErrorString.untranslatedText())) {
        return hidErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(outOfBoundsErrorString.untranslatedText())) {
        return outOfBoundsErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(unknownErrorString.untranslatedText())) {
        return unknownErrorString.toString();
    }
    return error.errorString();
}

QString HKUtils::headsetErrorToLocaleString(HKHeadsetError::Error error)
{
    switch (error) {
    case HKHeadsetError::Timeout:
        return timeErrorString.toString();
    case HKHeadsetError::DeviceOffline:
        return deviceOfflineErrorString.toString();
    case HKHeadsetError::ProtocolError:
        return protocolErrorString.toString();
    case HKHeadsetError::InvalidParameter:
        return invalidParameterErrorString.toString();
    case HKHeadsetError::NotSupported:
        return notSupportedErrorString.toString();
    case HKHeadsetError::PermissionDenied:
        return permissionDeniedErrorString.toString();
    case HKHeadsetError::UsbError:
        return usbErrorString.toString();
    case HKHeadsetError::HidError:
        return hidErrorString.toString();
    case HKHeadsetError::OutOfBounds:
        return outOfBoundsErrorString.toString();
    default:
        break;
    }
    return unknownErrorString.toString();
}
