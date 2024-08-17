#ifndef HEADSETCONTROLDEVICE_H
#define HEADSETCONTROLDEVICE_H

#include <QQmlEngine>
#include <QTimer>

#include <KSharedConfig>

#include "headsetcontroldevicebattery.h"

class HeadsetControl;
class HeadsetControlDevice : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Device)
    QML_UNCREATABLE("")

    Q_PROPERTY(HeadsetControlDeviceBattery *battery READ battery CONSTANT FINAL)
    Q_PROPERTY(int chatMix READ chatMix NOTIFY chatMixChanged FINAL)
    Q_PROPERTY(Capabilities capabilities READ capabilities NOTIFY capabilitiesChanged FINAL)
    Q_PROPERTY(QString device READ device NOTIFY deviceChanged FINAL)
    Q_PROPERTY(QString vendor READ vendor NOTIFY vendorChanged FINAL)
    Q_PROPERTY(QString product READ product NOTIFY productChanged FINAL)
    Q_PROPERTY(QString vendorId READ vendorId NOTIFY vendorIdChanged FINAL)
    Q_PROPERTY(QString productId READ productId NOTIFY productIdChanged FINAL)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)

    Q_PROPERTY(bool light READ light WRITE setLight NOTIFY lightChanged FINAL)
    Q_PROPERTY(bool voicePrompt READ voicePrompt WRITE setVoicePrompt NOTIFY voicePromptChanged FINAL)
    Q_PROPERTY(int sidetone READ sidetone WRITE setSidetone NOTIFY sidetoneChanged FINAL)
    Q_PROPERTY(int inactiveTime READ inactiveTime WRITE setInactiveTime NOTIFY inactiveTimeChanged FINAL)
    Q_PROPERTY(QString equalizer READ equalizer WRITE setEqualizer NOTIFY equalizerChanged FINAL)
    Q_PROPERTY(int equalizerPreset READ equalizerPreset WRITE setEqualizerPreset NOTIFY equalizerPresetChanged FINAL)
    Q_PROPERTY(bool rotateToMute READ rotateToMute WRITE setRotateToMute NOTIFY rotateToMuteChanged FINAL)
    Q_PROPERTY(int microphoneBrightness READ microphoneBrightness WRITE setMicrophoneBrightness NOTIFY microphoneBrightnessChanged FINAL)
    Q_PROPERTY(int microphoneVolume READ microphoneVolume WRITE setMicrophoneVolume NOTIFY microphoneVolumeChanged FINAL)
public:
    explicit HeadsetControlDevice(HeadsetControl *parent = nullptr);
    ~HeadsetControlDevice();

    enum Capability {
        Sidetone = 0x0001,
        Battery = 0x0002,
        NotificationSound = 0x0004,
        Light = 0x0008,
        InactiveTime = 0x0010,
        ChatMix = 0x0020,
        VoicePrompt = 0x0040,
        RotateToMute = 0x0080,
        EqualizerPreset = 0x0100,
        Equalizer = 0x0200,
        MicrophoneMuteLedBrightness = 0x0400,
        MicrophoneVolume = 0x0800
    };
    Q_DECLARE_FLAGS(Capabilities, Capability)
    Q_FLAG(Capabilities)

    enum Status { Success, Partial, Failure };
    Q_ENUM(Status)

    HeadsetControlDeviceBattery *battery() const;
    int chatMix() const;

    Capabilities capabilities() const;

    QString device() const;
    QString vendor() const;
    QString product() const;

    QString vendorId() const;
    QString productId() const;

    void update(const QVariantHash &hash);

    Status status() const;

    bool light() const;
    void setLight(bool state);

    bool voicePrompt() const;
    void setVoicePrompt(bool state);

    int sidetone() const;
    void setSidetone(int level);

    int inactiveTime() const;
    void setInactiveTime(int time);

    QString equalizer() const;
    void setEqualizer(const QString &eqString);

    int equalizerPreset() const;
    void setEqualizerPreset(int equalizerPreset);

    bool rotateToMute() const;
    void setRotateToMute(bool state);

    int microphoneBrightness() const;
    void setMicrophoneBrightness(int brightness);

    int microphoneVolume() const;
    void setMicrophoneVolume(int volume);

public Q_SLOTS:
    void playNotification(const QString &audioId);
    void reApply();

Q_SIGNALS:
    void chatMixChanged();
    void capabilitiesChanged();
    void deviceChanged();
    void vendorChanged();
    void productChanged();
    void vendorIdChanged();
    void productIdChanged();

    void errorOccurred(const QString &error);

    void statusChanged();

    void lightChanged();
    void voicePromptChanged();
    void sidetoneChanged();
    void inactiveTimeChanged();
    void equalizerChanged();
    void equalizerPresetChanged();
    void rotateToMuteChanged();
    void microphoneBrightnessChanged();
    void microphoneVolumeChanged();

private:
    Status m_status;
    QString m_device;
    QString m_vendor;
    QString m_product;

    QString m_vendorId;
    QString m_productId;

    HeadsetControlDeviceBattery *m_battery;
    int m_chatMix;

    Capabilities m_capabilities;

    HeadsetControl *m_headsetControl;
    KSharedConfig::Ptr m_config;

    void setChatMix(int newChatMix);
    void setCapabilities(const QStringList &newCapabilities);
    void setDevice(const QString &newDevice);
    void setVendor(const QString &newVendor);
    void setProduct(const QString &newProduct);
    void setVendorId(const QString &newVendorId);
    void setProductId(const QString &newProductId);
    void setStatus(const QString &newStatus);

    void applyLight(bool state, bool force = false);
    void applyVoicePrompt(bool state, bool force = false);
    void applySidetone(int level, bool force = false);
    void applyInactiveTime(int time, bool force = false);
    void applyEqualizer(const QString &eqString, bool force = false);
    void applyEqualizerPreset(int equalizerPreset, bool force = false);
    void applyRotateToMute(bool state, bool force = false);
    void applyMicrophoneBrightness(int brightness, bool force = false);
    void applyMicrophoneVolume(int volume, bool force = false);

    void writeConfig(const QString &key, const QVariant &value);
    QVariant readConfig(const QString &key, const QVariant &defaultValue) const;
};

#endif // HEADSETCONTROLDEVICE_H
