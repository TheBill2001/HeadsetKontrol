#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <KLocalizedString>
#include <KNotification>

#include "headsetcontrol.h"
#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

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

    connect(&m_timer, &QTimer::timeout, this, &HeadsetControl::refresh);

    connect(m_queue, &ProcessQueue::outputReady, this, &HeadsetControl::onUpdated);
    connect(m_queue, &ProcessQueue::errorOccurred, this, &HeadsetControl::processErrorOccurred);

    connect(&m_countdownRefreshTimer, &QTimer::timeout, this, &HeadsetControl::countDownTimeChanged);

    m_countdownRefreshTimer.setInterval(50); // This spikes CPU alot

    m_queueDelayTimer.setInterval(500);
    m_queueDelayTimer.setSingleShot(true);

    connect(&m_queueDelayTimer, &QTimer::timeout, this, &HeadsetControl::refresh);

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

    startCountDownUpdateTimer();

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

void HeadsetControl::queue(const QString &arg, const QVariant &value)
{
    m_delayedQueue.insert(arg, value);
    m_queueDelayTimer.start();
}

void HeadsetControl::stopCountDownUpdateTimer()
{
    m_countdownRefreshTimer.stop();
}

void HeadsetControl::startCountDownUpdateTimer()
{
    if (HeadsetKontrolConfig::instance()->showCountdownProgress()) {
        if (!m_countdownRefreshTimer.isActive())
            m_countdownRefreshTimer.start();
    }
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

    QList<HeadsetControlDevice *> updatedDevices;

    for (const auto &deviceVariant : std::as_const(devicesHash)) {
        auto deviceHash = deviceVariant.toHash();
        HeadsetControlDevice *device =
            getDevice(deviceHash.value(u"id_vendor"_s, QString()).toString(), deviceHash.value(u"id_product"_s, QString()).toString());

        if (device) {
            device->update(deviceHash);
        } else {
            device = new HeadsetControlDevice(this);
            device->update(deviceHash);
            device->reApply();
            addDevice(device);
        }

        updatedDevices.append(device);
    }

    for (auto device : std::as_const(m_devices)) {
        if (!updatedDevices.contains(device))
            removeDevice(device);
    }
}

void HeadsetControl::run()
{
    QStringList argsList = {u"--timeout"_s, u"500"_s, u"-o"_s, u"json"_s};

    if (!m_delayedQueue.isEmpty()) {
        for (auto i = m_delayedQueue.constBegin(); i != m_delayedQueue.constEnd(); ++i) {
            argsList.append(i.key());
            argsList.append(i.value().toString());
        }
        m_delayedQueue.clear();
    }

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

    if (!HeadsetKontrolConfig::instance()->deviceChangeNotification()) {
        return;
    }

    auto notification = new KNotification(u"newDeviceFound"_s, KNotification::CloseOnTimeout, this);
    notification->setAutoDelete(true);
    notification->setTitle(i18nc("@info:status", "New device found"));
    notification->setText(i18nc("@info:status", "Device %1 found.", device->device()));
    notification->setIconName(u"headsetkontrol"_s);
    notification->sendEvent();
}

void HeadsetControl::removeDevice(HeadsetControlDevice *device)
{
    if (!m_devices.contains(device))
        return;

    m_devices.removeAll(device);
    Q_EMIT devicesChanged();

    auto notification = new KNotification(u"deviceRemoved"_s, KNotification::CloseOnTimeout, this);
    notification->setAutoDelete(true);
    notification->setTitle(i18nc("@info:status", "Device removed"));
    notification->setText(i18nc("@info:status", "Device %1 removed.", device->device()));
    notification->setIconName(u"headsetkontrol"_s);
    notification->sendEvent();

    device->deleteLater();
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
