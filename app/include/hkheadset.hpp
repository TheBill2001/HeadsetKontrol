// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSET_HPP
#define HKHEADSET_HPP

#include "headsetkontrol_export.hpp"

#include <QObject>

class HKBattery;
class HKChatMix;
class HKHeadsetError;
class HKHeadsetId;

class HKHeadsetPrivate;
class HK_EXPORT HKHeadset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HKHeadsetId id READ id CONSTANT FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL)
    Q_PROPERTY(HKHeadset::Capabilities capabilities READ capabilities NOTIFY capabilitiesChanged BINDABLE bindableCapabilities FINAL)
    Q_PROPERTY(QList<HKHeadsetError> errors READ errors NOTIFY errorsChanged BINDABLE bindableErrors FINAL)
    Q_PROPERTY(HKBattery battery READ battery NOTIFY batteryChanged BINDABLE bindableBattery FINAL)
    Q_PROPERTY(HKChatMix chatmix READ chatMix NOTIFY chatMixChanged BINDABLE bindableChatMix FINAL)

    Q_DECLARE_PRIVATE(HKHeadset)
    HKHeadsetPrivate *const d_ptr;

    friend class HKHeadsetControlPrivate;

    class InitData;
    explicit HKHeadset(InitData &&initData);

public:
    enum Capability : quint16 {
        UnknownCapability = 0,
        SidetoneCapability = 1,
        BatteryStatusCapability = 2,
        NotificationSoundCapability = 4,
        LightsCapability = 8,
        InactiveTimeCapability = 16,
        ChatMixStatusCapability = 32,
        VoicePromptsCapability = 64,
        RotateToMuteCapability = 128,
        EqualizerPresetCapability = 256,
        EqualizerCapability = 512,
        ParametricEqualizerCapability = 1024,
        MicrophoneMuteLedBrightnessCapability = 2048,
        MicrophoneVolumeCapability = 4096,
        VolumeLimiterCapability = 8192,
        BluetoothWhenPoweredOnCapability = 16384,
        BluetoothCallVolumeCapability = 32768,
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)

    ~HKHeadset() override;

    [[nodiscard]] HKHeadsetId id() const noexcept;
    [[nodiscard]] QString name() const noexcept;

    [[nodiscard]] QBindable<Capabilities> bindableCapabilities() const;
    [[nodiscard]] Capabilities capabilities() const;

    [[nodiscard]] QBindable<QList<HKHeadsetError>> bindableErrors() const;
    [[nodiscard]] QList<HKHeadsetError> errors() const;

    [[nodiscard]] QBindable<HKBattery> bindableBattery() const;
    [[nodiscard]] HKBattery battery() const;

    [[nodiscard]] QBindable<HKChatMix> bindableChatMix() const;
    [[nodiscard]] HKChatMix chatMix() const;

public Q_SLOTS:
    void refresh();

Q_SIGNALS:
    void capabilitiesChanged(HKHeadset::Capabilities, QPrivateSignal);
    void errorsChanged(const QList<HKHeadsetError> &, QPrivateSignal);
    void errorsOccurred(const QList<HKHeadsetError> &, QPrivateSignal);
    void batteryChanged(const HKBattery &, QPrivateSignal);
    void chatMixChanged(const HKChatMix &, QPrivateSignal);
};

Q_MOC_INCLUDE("hkbattery.hpp")
Q_MOC_INCLUDE("hkchatmix.hpp")
Q_MOC_INCLUDE("hkheadseterror.hpp")
Q_MOC_INCLUDE("hkheadsetid.hpp")

#endif // HKHEADSET_HPP
