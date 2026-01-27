// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkqmlutils.hpp"

#include "hkheadset.hpp"

#include <QCollator>
#include <QLocale>

#include <KLocalizedString>

using namespace Qt::StringLiterals;

qint32 HKQmlUtils::localeStringToMilliseconds(const QString &text, const QLocale &locale)
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

QString HKQmlUtils::millisecondsToLocaleString(int value)
{
    return i18ncp("@item:valuesuffix", "%1 millisecond", "%1 milliseconds", value);
}

qsizetype HKQmlUtils::countCapabilities(quint64 caps)
{
    return countFlags(HKHeadset::Capabilities::fromInt(caps));
}

QString HKQmlUtils::capabilityLocaleString(quint64 cap)
{
    switch (cap) {
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

QStringList HKQmlUtils::capabilityLocaleStringList(quint64 caps)
{
    HKHeadset::Capabilities _caps = HKHeadset::Capabilities::fromInt(caps);
    QStringList stringList;
    const auto metaEnum = QMetaEnum::fromType<HKHeadset::Capabilities>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        const auto value = HKHeadset::Capability(metaEnum.value64(i).value_or(0));
        if (_caps.testFlag(value)) {
            stringList << capabilityLocaleString(value);
        }
    }
    QCollator collator;
    std::ranges::sort(stringList, collator);
    return stringList;
}
