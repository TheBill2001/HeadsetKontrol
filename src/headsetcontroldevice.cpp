#include "headsetcontroldevice.h"

#include <QTimer>

#include <KLocalizedString>

#include "headsetcontrol.h"
#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

HeadsetControlDevice::HeadsetControlDevice(HeadsetControl *parent)
    : QObject{parent}
    , m_battery{new HeadsetControlDeviceBattery(this)}
    , m_headsetControl{parent}
{
    m_reApplyAction = new QAction(this);
    m_reApplyAction->setIcon(QIcon::fromTheme(u"dialog-ok-apply"_s));
    m_reApplyAction->setText(i18nc("@action apply all", "Re-apply all"));
    m_reApplyAction->setToolTip(i18nc("@info:tooltip apply all", "Re-apply all settings"));
    connect(m_reApplyAction, &QAction::triggered, this, &HeadsetControlDevice::reApply);

    m_setPrimaryAction = new QAction(this);
    m_setPrimaryAction->setIcon(QIcon::fromTheme(u"favorite"_s));
    m_setPrimaryAction->setCheckable(true);
    m_setPrimaryAction->setText(i18nc("@action", "Mark as primary"));
    m_setPrimaryAction->setToolTip(i18nc("@info:tooltip apply all", "Mark device as primary device"));
    connect(m_setPrimaryAction, &QAction::triggered, this, &HeadsetControlDevice::setPrimary);
    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::PrimaryChanged, this, [this]() {
        bool isPrimary = false;
        auto primary = HeadsetKontrolConfig::instance()->primary();
        if (primary.length() > 0 && primary.at(0) == vendorId()) {
            if (primary.length() > 1 && primary.at(1) == productId()) {
                isPrimary = true;
            }
        }
        m_setPrimaryAction->setChecked(isPrimary);
    });
}

HeadsetControlDevice::~HeadsetControlDevice()
{
}

HeadsetControlDeviceBattery *HeadsetControlDevice::battery() const
{
    return m_battery;
}

int HeadsetControlDevice::chatMix() const
{
    return m_chatMix;
}

void HeadsetControlDevice::setChatMix(int newChatMix)
{
    if (m_chatMix == newChatMix)
        return;
    m_chatMix = newChatMix;
    Q_EMIT chatMixChanged();
}

HeadsetControlDevice::Capabilities HeadsetControlDevice::capabilities() const
{
    return m_capabilities;
}

void HeadsetControlDevice::setCapabilities(const QStringList &newCapabilities)
{
    Capabilities cap;
    if (newCapabilities.contains(u"CAP_SIDETONE"_s)) {
        cap |= Sidetone;
    }

    if (newCapabilities.contains(u"CAP_BATTERY_STATUS"_s)) {
        cap |= Battery;
    }

    if (newCapabilities.contains(u"CAP_NOTIFICATION_SOUND"_s)) {
        cap |= NotificationSound;
    }

    if (newCapabilities.contains(u"CAP_LIGHTS"_s)) {
        cap |= Light;
    }

    if (newCapabilities.contains(u"CAP_INACTIVE_TIME"_s)) {
        cap |= InactiveTime;
    }

    if (newCapabilities.contains(u"CAP_CHATMIX_STATUS"_s)) {
        cap |= ChatMix;
    }

    if (newCapabilities.contains(u"CAP_VOICE_PROMPTS"_s)) {
        cap |= VoicePrompt;
    }

    if (newCapabilities.contains(u"CAP_ROTATE_TO_MUTE"_s)) {
        cap |= RotateToMute;
    }

    if (newCapabilities.contains(u"CAP_EQUALIZER_PRESET"_s)) {
        cap |= EqualizerPreset;
    }

    if (newCapabilities.contains(u"CAP_EQUALIZER"_s)) {
        cap |= Equalizer;
    }

    if (newCapabilities.contains(u"CAP_MICROPHONE_MUTE_LED_BRIGHTNESS"_s)) {
        cap |= MicrophoneMuteLedBrightness;
    }

    if (newCapabilities.contains(u"CAP_MICROPHONE_VOLUME"_s)) {
        cap |= MicrophoneVolume;
    }

    if (m_capabilities == cap)
        return;
    m_capabilities = cap;
    Q_EMIT capabilitiesChanged();
}

QString HeadsetControlDevice::device() const
{
    return m_device;
}

void HeadsetControlDevice::setDevice(const QString &newDevice)
{
    if (m_device == newDevice)
        return;
    m_device = newDevice;
    Q_EMIT deviceChanged();
}

QString HeadsetControlDevice::vendor() const
{
    return m_vendor;
}

void HeadsetControlDevice::setVendor(const QString &newVendor)
{
    if (m_vendor == newVendor)
        return;
    m_vendor = newVendor;
    Q_EMIT vendorChanged();
}

QString HeadsetControlDevice::product() const
{
    return m_product;
}

QString HeadsetControlDevice::vendorId() const
{
    return m_vendorId;
}

void HeadsetControlDevice::setVendorId(const QString &newVendorId)
{
    if (m_vendorId == newVendorId)
        return;
    m_vendorId = newVendorId;
    Q_EMIT vendorIdChanged();
}

QString HeadsetControlDevice::productId() const
{
    return m_productId;
}

void HeadsetControlDevice::update(const QVariantHash &hash)
{
    setStatus(hash.value(u"status"_s, QString()).toString());
    setDevice(hash.value(u"device"_s, QString()).toString());
    setVendor(hash.value(u"vendor"_s, QString()).toString());
    setProduct(hash.value(u"product"_s, QString()).toString());
    setVendorId(hash.value(u"id_vendor"_s, QString()).toString());
    setProductId(hash.value(u"id_product"_s, QString()).toString());
    setChatMix(hash.value(u"chatmix"_s, 0).toInt());
    setCapabilities(hash.value(u"capabilities"_s, QStringList()).toStringList());
    battery()->update(hash.value(u"battery"_s, QStringList()).toHash());

    if (product().length() > 0) {
        m_setPrimaryAction->setData(product());
    } else {
        m_setPrimaryAction->setData(device());
    }

    bool isPrimary = false;
    auto primary = HeadsetKontrolConfig::instance()->primary();
    if (primary.length() > 0 && primary.at(0) == vendorId()) {
        if (primary.length() > 1 && primary.at(1) == productId()) {
            isPrimary = true;
        }
    }
    m_setPrimaryAction->setChecked(isPrimary);

    auto errors = hash.value(u"errors"_s, QString()).toStringList();
    for (auto const &error : std::as_const(errors)) {
        Q_EMIT errorOccurred(error);
    }
}

HeadsetControlDevice::Status HeadsetControlDevice::status() const
{
    return m_status;
}

bool HeadsetControlDevice::light() const
{
    return readConfig(u"Light"_s, true).toBool();
}

void HeadsetControlDevice::setLight(bool state)
{
    applyLight(state);
}

bool HeadsetControlDevice::voicePrompt() const
{
    return readConfig(u"VoicePrompt"_s, true).toBool();
}

void HeadsetControlDevice::setVoicePrompt(bool state)
{
    applyVoicePrompt(state);
}

int HeadsetControlDevice::sidetone() const
{
    return readConfig(u"Sidetone"_s, 0).toInt();
}

void HeadsetControlDevice::setSidetone(int level)
{
    applySidetone(level);
}

int HeadsetControlDevice::inactiveTime() const
{
    return readConfig(u"InactiveTime"_s, 10).toInt();
}

void HeadsetControlDevice::setInactiveTime(int time)
{
    applyInactiveTime(time);
}

QString HeadsetControlDevice::equalizer() const
{
    return readConfig(u"Equalizer"_s, QString()).toString();
}

void HeadsetControlDevice::setEqualizer(const QString &eqString)
{
    applyEqualizer(eqString);
}

int HeadsetControlDevice::equalizerPreset() const
{
    return readConfig(u"EqualizerPreset"_s, 0).toInt();
}

void HeadsetControlDevice::setEqualizerPreset(int equalizerPreset)
{
    applyEqualizerPreset(equalizerPreset);
}

bool HeadsetControlDevice::rotateToMute() const
{
    return readConfig(u"RotateToMute"_s, true).toBool();
}

void HeadsetControlDevice::setRotateToMute(bool state)
{
    applyRotateToMute(state);
}

int HeadsetControlDevice::microphoneBrightness() const
{
    return readConfig(u"MicrophonBrightness"_s, 3).toInt();
}

void HeadsetControlDevice::setMicrophoneBrightness(int brightness)
{
    applyMicrophoneBrightness(brightness);
}

int HeadsetControlDevice::microphoneVolume() const
{
    return readConfig(u"MicrophonVolume"_s, 128).toInt();
}

void HeadsetControlDevice::setMicrophoneVolume(int volume)
{
    applyMicrophoneVolume(volume);
}

QAction *HeadsetControlDevice::reApplyAction() const
{
    return m_reApplyAction;
}

QAction *HeadsetControlDevice::setPrimaryAction() const
{
    return m_setPrimaryAction;
}

void HeadsetControlDevice::playNotification(const QString &audioId)
{
    if (m_capabilities.testFlag(NotificationSound)) {
        if (audioId.isEmpty())
            return;
        m_headsetControl->queue(u"--notificate"_s, audioId.isEmpty());
    }
}

void HeadsetControlDevice::reApply()
{
    applyLight(light(), true);
    applyVoicePrompt(voicePrompt(), true);
    applySidetone(sidetone(), true);
    applyInactiveTime(inactiveTime(), true);
    applyEqualizer(equalizer(), true);
    applyEqualizerPreset(equalizerPreset(), true);
    applyRotateToMute(rotateToMute(), true);
    applyMicrophoneBrightness(microphoneBrightness(), true);
    applyMicrophoneVolume(microphoneVolume(), true);
}

void HeadsetControlDevice::setPrimary()
{
    HeadsetKontrolConfig::instance()->setPrimary({vendorId(), productId()});
}

void HeadsetControlDevice::setStatus(const QString &newStatus)
{
    Status stat = Failure;
    if (newStatus == u"partial"_s)
        stat = Partial;
    else if (newStatus == u"success"_s)
        stat = Success;

    if (m_status == stat)
        return;
    m_status = stat;
    Q_EMIT statusChanged();
}

void HeadsetControlDevice::applyLight(bool state, bool force)
{
    if (m_capabilities.testFlag(Light)) {
        if (light() == state && !force)
            return;

        writeConfig(u"Light"_s, state);
        Q_EMIT lightChanged();

        m_headsetControl->queue(u"--light"_s, state ? u"1"_s : u"0"_s);
    }
}

void HeadsetControlDevice::applyVoicePrompt(bool state, bool force)
{
    if (m_capabilities.testFlag(VoicePrompt)) {
        if (voicePrompt() == state && !force)
            return;

        writeConfig(u"VoicePrompt"_s, state);
        Q_EMIT voicePromptChanged();

        m_headsetControl->queue(u"--voice-prompt"_s, state ? u"1"_s : u"0"_s);
    }
}

void HeadsetControlDevice::applySidetone(int level, bool force)
{
    if (m_capabilities.testFlag(Sidetone)) {
        if (sidetone() == level && !force)
            return;

        writeConfig(u"Sidetone"_s, level);
        Q_EMIT sidetoneChanged();

        m_headsetControl->queue(u"--sidetone"_s, QString::number(level));
    }
}

void HeadsetControlDevice::applyInactiveTime(int time, bool force)
{
    if (m_capabilities.testFlag(InactiveTime)) {
        if (inactiveTime() == time && !force)
            return;

        writeConfig(u"InactiveTime"_s, time);
        Q_EMIT inactiveTimeChanged();

        m_headsetControl->queue(u"--inactive-time"_s, QString::number(time));
    }
}

void HeadsetControlDevice::applyEqualizer(const QString &eqString, bool force)
{
    if (m_capabilities.testFlag(Equalizer)) {
        if (eqString.isEmpty())
            return;

        if (equalizer() == eqString && !force)
            return;

        writeConfig(u"Equalizer"_s, eqString);
        Q_EMIT equalizerChanged();

        m_headsetControl->queue(u"--equalizer"_s, eqString);
    }
}

void HeadsetControlDevice::applyEqualizerPreset(int equalizerPreset, bool force)
{
    if (m_capabilities.testFlag(EqualizerPreset)) {
        if (equalizerPreset < 0 || equalizerPreset > 3)
            return;

        if (this->equalizerPreset() == equalizerPreset && !force)
            return;

        writeConfig(u"EqualizerPreset"_s, equalizerPreset);
        Q_EMIT equalizerPresetChanged();

        m_headsetControl->queue(u"--equalizer-preset"_s, QString::number(equalizerPreset));
    }
}

void HeadsetControlDevice::applyRotateToMute(bool state, bool force)
{
    if (m_capabilities.testFlag(RotateToMute)) {
        if (rotateToMute() == state && !force)
            return;

        writeConfig(u"RotateToMute"_s, state);
        Q_EMIT rotateToMuteChanged();

        m_headsetControl->queue(u"--rotate-to-mute"_s, state ? u"1"_s : u"0"_s);
    }
}

void HeadsetControlDevice::applyMicrophoneBrightness(int brightness, bool force)
{
    if (m_capabilities.testFlag(MicrophoneMuteLedBrightness)) {
        if (brightness < 0 || brightness > 3)
            return;

        if (microphoneBrightness() == brightness && !force)
            return;

        writeConfig(u"MicrophonBrightness"_s, brightness);
        Q_EMIT microphoneBrightnessChanged();

        m_headsetControl->queue(u"--microphone-mute-led-brightness"_s, QString::number(brightness));
    }
}

void HeadsetControlDevice::applyMicrophoneVolume(int volume, bool force)
{
    if (m_capabilities.testFlag(MicrophoneVolume)) {
        if (volume < 0 || volume > 128)
            return;

        if (microphoneVolume() == volume && !force)
            return;

        writeConfig(u"MicrophonVolume"_s, volume);
        Q_EMIT microphoneVolumeChanged();

        m_headsetControl->queue(u"--microphone-volume"_s, QString::number(volume));
    }
}

void HeadsetControlDevice::writeConfig(const QString &key, const QVariant &value)
{
    auto vendorConfigGroup = HeadsetKontrolConfig::instance()->config()->group(vendorId());
    auto productConfigGroup = vendorConfigGroup.group(productId());
    productConfigGroup.writeEntry(key, value);
}

QVariant HeadsetControlDevice::readConfig(const QString &key, const QVariant &defaultValue) const
{
    auto vendorConfigGroup = HeadsetKontrolConfig::instance()->config()->group(vendorId());
    auto productConfigGroup = vendorConfigGroup.group(productId());
    return productConfigGroup.readEntry(key, defaultValue);
}

void HeadsetControlDevice::setProductId(const QString &newProductId)
{
    if (m_productId == newProductId)
        return;
    m_productId = newProductId;
    Q_EMIT productIdChanged();
}

void HeadsetControlDevice::setProduct(const QString &newProduct)
{
    if (m_product == newProduct)
        return;
    m_product = newProduct;
    Q_EMIT productChanged();
}
