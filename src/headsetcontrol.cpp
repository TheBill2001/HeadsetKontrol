#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "config.h"
#include "headsetcontrol.h"

using namespace Qt::Literals::StringLiterals;

HeadsetControlDeviceBattery::HeadsetControlDeviceBattery(HeadsetControlDevice *parent)
    : QObject{parent}
{
}

HeadsetControlDeviceBattery::~HeadsetControlDeviceBattery()
{
}

HeadsetControlDeviceBattery::Status HeadsetControlDeviceBattery::status() const
{
    return m_status;
}

void HeadsetControlDeviceBattery::setStatus(const QString &newStatus)
{
    Status stat = Unavailable;
    if (newStatus == u"BATTERY_CHARGING"_s) {
        stat = Charging;
    } else if (newStatus == u"BATTERY_AVAILABLE"_s) {
        stat = Available;
    } else if (newStatus == u"BATTERY_HIDERROR"_s) {
        stat = HidError;
    } else if (newStatus == u"BATTERY_TIMEOUT"_s) {
        stat = Timeout;
    }

    if (m_status == stat)
        return;
    m_status = stat;
    Q_EMIT statusChanged();
}

int HeadsetControlDeviceBattery::level() const
{
    return m_level;
}

void HeadsetControlDeviceBattery::update(const QVariantHash &hash)
{
    setStatus(hash.value(u"status"_s, QString()).toString());
    setLevel(hash.value(u"level"_s, -1).toInt());
}

void HeadsetControlDeviceBattery::setLevel(int newLevel)
{
    if (m_level == newLevel)
        return;
    m_level = newLevel;
    Q_EMIT levelChanged();
}

HeadsetControlDevice::HeadsetControlDevice(HeadsetControl *parent)
    : QObject{parent}
    , m_battery{new HeadsetControlDeviceBattery(this)}
    , m_headsetControl{parent}
    , m_config{HeadsetKontrolConfig::instance()->sharedConfig()}
{
    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::configChanged, this, [=]() {
        qInfo() << "HAHA!";
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
    return readConfig(u"light"_s, true).toBool();
}

void HeadsetControlDevice::setLight(bool state)
{
    if (m_capabilities.testFlag(Light)) {
        writeConfig(u"light"_s, state);
        m_headsetControl->start({u"--light"_s, state ? u"1"_s : u"0"_s});
    }
}

bool HeadsetControlDevice::voicePrompt() const
{
    return readConfig(u"voicePrompt"_s, true).toBool();
}

void HeadsetControlDevice::setVoicePrompt(bool state)
{
    if (m_capabilities.testFlag(VoicePrompt)) {
        writeConfig(u"voicePrompt"_s, state);
        m_headsetControl->start({u"--voice-prompt"_s, state ? u"1"_s : u"0"_s});
    }
}

int HeadsetControlDevice::inactiveTime() const
{
    return readConfig(u"inactiveTime"_s, 10).toInt();
}

void HeadsetControlDevice::setInactiveTime(int time)
{
    if (m_capabilities.testFlag(InactiveTime)) {
        writeConfig(u"inactiveTime"_s, time);
        m_headsetControl->start({u"--inactive-time"_s, QString::number(time)});
    }
}

QString HeadsetControlDevice::equalizer() const
{
    return readConfig(u"equalizer"_s, QString()).toString();
}

void HeadsetControlDevice::setEqualizer(const QString &eqString)
{
    if (m_capabilities.testFlag(Equalizer)) {
        writeConfig(u"equalizer"_s, eqString);
        m_headsetControl->start({u"--equalizer"_s, eqString});
    }
}

int HeadsetControlDevice::equalizerPreset() const
{
    return readConfig(u"equalizerPreset"_s, 0).toInt();
}

void HeadsetControlDevice::setEqualizerPreset(int equalizerPreset)
{
    if (m_capabilities.testFlag(EqualizerPreset)) {
        if (equalizerPreset < 0 || equalizerPreset > 3)
            return;

        writeConfig(u"equalizerPreset"_s, equalizerPreset);
        m_headsetControl->start({u"--equalizer-preset"_s, QString::number(equalizerPreset)});
    }
}

bool HeadsetControlDevice::rotateToMute() const
{
    return readConfig(u"rotateToMute"_s, true).toBool();
}

void HeadsetControlDevice::setRotateToMute(bool state)
{
    if (m_capabilities.testFlag(RotateToMute)) {
        writeConfig(u"rotateToMute"_s, state);
        m_headsetControl->start({u"--rotate-to-mute"_s, state ? u"1"_s : u"0"_s});
    }
}

int HeadsetControlDevice::microphoneBrightness() const
{
    return readConfig(u"microphonBrightness"_s, 3).toInt();
}

void HeadsetControlDevice::setMicrophoneBrightness(int brightness)
{
    if (m_capabilities.testFlag(MicrophoneMuteLedBrightness)) {
        if (brightness < 0 || brightness > 3)
            return;

        writeConfig(u"equalizerPreset"_s, brightness);
        m_headsetControl->start({u"--microphone-mute-led-brightness"_s, QString::number(brightness)});
    }
}

int HeadsetControlDevice::microphoneVolume() const
{
    return readConfig(u"microphonVolume"_s, 128).toInt();
}

void HeadsetControlDevice::setMicrophoneVolume(int volume)
{
    if (m_capabilities.testFlag(MicrophoneVolume)) {
        if (volume < 0 || volume > 128)
            return;

        writeConfig(u"equalizerPreset"_s, volume);
        m_headsetControl->start({u"--microphone-volume"_s, QString::number(volume)});
    }
}

void HeadsetControlDevice::playNotification(const QString &audioId)
{
    if (m_capabilities.testFlag(NotificationSound)) {
        m_headsetControl->start({u"--notificate"_s, audioId});
    }
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

void HeadsetControlDevice::writeConfig(const QString &key, const QVariant &value)
{
    auto vendorConfigGroup = m_config->group(vendorId());
    auto productConfigGroup = vendorConfigGroup.group(productId());
    productConfigGroup.writeEntry(key, value);
    productConfigGroup.sync();
    HeadsetKontrolConfig::instance()->save();
}

QVariant HeadsetControlDevice::readConfig(const QString &key, const QVariant &defaultValue) const
{
    auto vendorConfigGroup = m_config->group(vendorId());
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

HeadsetControl::HeadsetControl(QObject *parent)
    : QObject{parent}
    , m_queue{new ProcessQueue(this)}
{
    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::ExecutablePathChanged, this, [this]() {
        this->start();
    });
    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::UpdateRateChanged, this, [this]() {
        this->start();
    });

    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::ShowCountdownProgressChanged, this, [=]() {
        if (HeadsetKontrolConfig::instance()->showCountdownProgress())
            startCountDownUpdateTimer();
        else
            stopCountDownUpdateTimer();
    });

    m_timer.setParent(this);
    m_timer.setSingleShot(false);
    m_timer.setInterval(HeadsetKontrolConfig::instance()->updateRate());

    connect(&m_timer, &QTimer::timeout, this, [this]() {
        this->run();
    });

    connect(m_queue, &ProcessQueue::outputReady, this, &HeadsetControl::onUpdated);
    connect(m_queue, &ProcessQueue::errorOccurred, this, &HeadsetControl::processErrorOccurred);

    connect(&m_countdownRefreshTimer, &QTimer::timeout, this, &HeadsetControl::countDownTimeChanged);

    m_countdownRefreshTimer.setInterval(50); // This spikes CPU alot

    QTimer::singleShot(0, this, [this]() {
        this->start();
    });
}

QString HeadsetControl::version() const
{
    return m_version;
}

void HeadsetControl::setVersion(const QString &newVersion)
{
    if (m_version == newVersion)
        return;
    m_version = newVersion;
    Q_EMIT versionChanged();
}

QString HeadsetControl::apiVersion() const
{
    return m_apiVersion;
}

void HeadsetControl::setApiVersion(const QString &newApiVersion)
{
    if (m_apiVersion == newApiVersion)
        return;
    m_apiVersion = newApiVersion;
    Q_EMIT apiVersionChanged();
}

QString HeadsetControl::hidApiVersion() const
{
    return m_hidApiVersion;
}

int HeadsetControl::updateRate() const
{
    return m_updateRate;
}

void HeadsetControl::setUpdateRate(int newUpdateRate)
{
    if (m_updateRate == newUpdateRate)
        return;
    m_updateRate = newUpdateRate;
    Q_EMIT updateRateChanged();
}

void HeadsetControl::start(const QStringList &args)
{
    if (HeadsetKontrolConfig::instance()->executablePath().isEmpty() || HeadsetKontrolConfig::instance()->updateRate() <= 0) {
        stop();
        return;
    }

    m_timer.start(HeadsetKontrolConfig::instance()->updateRate());

    if (HeadsetKontrolConfig::instance()->showCountdownProgress())
        m_countdownRefreshTimer.start();

    Q_EMIT isRunningChanged();
    run(args);
}

void HeadsetControl::stop()
{
    m_timer.stop();
    m_queue->stop();
    Q_EMIT isRunningChanged();
}

void HeadsetControl::refresh()
{
    if (isRunning()) {
        start();
    } else {
        run();
    }
}

void HeadsetControl::stopCountDownUpdateTimer()
{
    m_countdownRefreshTimer.stop();
}

void HeadsetControl::startCountDownUpdateTimer()
{
    m_countdownRefreshTimer.start();
}

void HeadsetControl::onUpdated(const QByteArray &data)
{
    if (data.isEmpty()) {
        clearDevices();
        stop();
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
    QString errorString;
    bool hasError = false;

    if (jsonError.error != QJsonParseError::NoError) {
        qWarning().noquote() << u"Failed to parse headsetcontrol output, JSON error '%1' at position '%2': '%3'."_s.arg(jsonError.errorString(),
                                                                                                                        QString::number(jsonError.offset),
                                                                                                                        QString::fromUtf8(data));
        errorString =
            u"Failed to parse headsetcontrol output, JSON error '%1' at position '%2'."_s.arg(jsonError.errorString(), QString::number(jsonError.offset));
        hasError = true;
    }

    if (!hasError && !doc.isObject()) {
        qWarning().noquote() << u"Failed to parse headsetcontrol output, Expecting root element to be an object: '%1'."_s.arg(QString::fromUtf8(data));
        errorString = u"Failed to parse headsetcontrol output, Expecting root element to be an object."_s;
        hasError = true;
    }

    if (hasError) {
        stop();
        Q_EMIT parsingErrorOccurred(errorString);
        return;
    }

    auto rootHash = doc.object().toVariantHash();
    setVersion(rootHash.value(u"version"_s, QString()).toString());
    setApiVersion(rootHash.value(u"api_version"_s, QString()).toString());
    setHidApiVersion(rootHash.value(u"hidapi_version"_s, QString()).toString());

    auto devicesHash = rootHash.value(u"devices"_s, QVariantList()).toList();
    for (const auto &deviceVariant : std::as_const(devicesHash)) {
        auto deviceHash = deviceVariant.toHash();
        HeadsetControlDevice *device =
            getDevice(deviceHash.value(u"id_vendor"_s, QString()).toString(), deviceHash.value(u"id_product"_s, QString()).toString());

        if (device) {
            device->update(deviceHash);
        } else {
            device = new HeadsetControlDevice(this);
            device->update(deviceHash);
            addDevice(device);
        }
    }
}

void HeadsetControl::run(const QStringList &args)
{
    QStringList argsList = {u"--test-device"_s, u"-o"_s, u"json"_s};
    if (!args.isEmpty())
        argsList.append(args);
    m_queue->addProcess(HeadsetKontrolConfig::instance()->executablePath(), argsList);
    m_queue->start();
}

QList<HeadsetControlDevice *> HeadsetControl::devices() const
{
    return m_devices;
}

qint32 HeadsetControl::countDownTime() const
{
    return m_timer.remainingTime();
}

bool HeadsetControl::isRunning() const
{
    return m_timer.isActive();
}

void HeadsetControl::setHidApiVersion(const QString &newHidApiVersion)
{
    if (m_hidApiVersion == newHidApiVersion)
        return;
    m_hidApiVersion = newHidApiVersion;
    Q_EMIT hidApiVersionChanged();
}

void HeadsetControl::addDevice(HeadsetControlDevice *device)
{
    if (m_devices.contains(device))
        return;
    connect(device, &HeadsetControlDevice::errorOccurred, this, [=](const QString &error) {
        Q_EMIT this->deviceErrorOccurred(device, error);
    });
    m_devices.append(device);
    Q_EMIT devicesChanged();
}

void HeadsetControl::removeDevice(HeadsetControlDevice *device)
{
    if (!m_devices.contains(device))
        return;
    m_devices.removeAll(device);
    Q_EMIT devicesChanged();
}

void HeadsetControl::clearDevices()
{
    setVersion({});
    setApiVersion({});
    setHidApiVersion({});

    if (m_devices.isEmpty())
        return;

    while (!m_devices.isEmpty()) {
        auto device = m_devices.last();
        m_devices.pop_back();
        device->deleteLater();
    }
    Q_EMIT devicesChanged();
}

HeadsetControlDevice *HeadsetControl::getDevice(const QString &vendorId, const QString &productId)
{
    for (auto device : std::as_const(m_devices)) {
        if (device->vendorId() == vendorId && device->productId() == productId)
            return device;
    }
    return nullptr;
}
