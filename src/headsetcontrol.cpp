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
{
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
        cap |= LED;
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
    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::ExecutablePathChanged, this, &HeadsetControl::start);
    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::UpdateRateChanged, this, &HeadsetControl::start);

    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::ShowCountdownProgressChanged, this, [=]() {
        if (HeadsetKontrolConfig::instance()->showCountdownProgress())
            startCountDownUpdateTimer();
        else
            stopCountDownUpdateTimer();
    });

    m_timer.setParent(this);
    m_timer.setSingleShot(false);
    m_timer.setInterval(HeadsetKontrolConfig::instance()->updateRate());

    connect(&m_timer, &QTimer::timeout, this, &HeadsetControl::run);

    connect(m_queue, &ProcessQueue::outputReady, this, &HeadsetControl::onUpdated);
    connect(m_queue, &ProcessQueue::errorOccurred, this, &HeadsetControl::processErrorOccurred);

    connect(&m_countdownRefreshTimer, &QTimer::timeout, this, &HeadsetControl::countDownTimeChanged);

    m_countdownRefreshTimer.setInterval(50); // This spikes CPU alot

    QTimer::singleShot(0, this, &HeadsetControl::start);
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

void HeadsetControl::start()
{
    if (HeadsetKontrolConfig::instance()->executablePath().isEmpty() || HeadsetKontrolConfig::instance()->updateRate() <= 0) {
        stop();
        return;
    }

    m_timer.start(HeadsetKontrolConfig::instance()->updateRate());

    if (HeadsetKontrolConfig::instance()->showCountdownProgress())
        m_countdownRefreshTimer.start();

    Q_EMIT isRunningChanged();
    run();
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

void HeadsetControl::run()
{
    m_queue->addProcess(HeadsetKontrolConfig::instance()->executablePath(), {u"--test-device"_s, u"-o"_s, u"json"_s});
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
