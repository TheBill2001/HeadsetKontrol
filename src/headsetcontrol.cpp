#include <QDebug>

#include "headsetcontrol.h"

HeadsetControl::HeadsetControl(const QString &path, QObject *parent)
    : QObject{parent}
    , m_path{path}
    , m_battery{-2}
    , m_chatMix{-1}
    , m_pathValid{false}
    , m_queue{this}
{
    connect(&m_queue, &ProcessQueue::outputReady, this, &HeadsetControl::outputReady);
    connect(this, &HeadsetControl::pathChecked, this, &HeadsetControl::onPathChecked);
    connect(this, &HeadsetControl::capabilitiesQueried, this, &HeadsetControl::onCapabilitiesUpdated);

    if (!m_path.isEmpty())
        checkPath();

    qDebug() << "HeadsetControl instance created: " << this;
}

QString HeadsetControl::getName() const
{
    return m_name;
}

int HeadsetControl::getBattery() const
{
    return m_battery;
}

int HeadsetControl::getChatMix() const
{
    return m_chatMix;
}

QList<HeadsetControl::Capabilities> HeadsetControl::getCapabilities() const
{
    return m_capabilities;
}

// QVariantList HeadsetControl::getCapabilitiesVariantList()
//{
//     return QVariantList(getCapabilities());
// }

void HeadsetControl::setPath(const QString &path)
{
    if (path != m_path) {
        m_path = path;
        Q_EMIT pathChanged(m_path);
    }
}

QString HeadsetControl::getPath() const
{
    return m_path;
}

bool HeadsetControl::isPathValid() const
{
    return m_pathValid;
}

bool HeadsetControl::hasBatteryCapability() const
{
    return m_capabilities.contains(Capabilities::Battery);
}

bool HeadsetControl::hasSidetoneCapability() const
{
    return m_capabilities.contains(Capabilities::Sidetone);
}

bool HeadsetControl::hasNotificationSoundCapability() const
{
    return m_capabilities.contains(Capabilities::NotificationSound);
}

bool HeadsetControl::hasLedCapability() const
{
    return m_capabilities.contains(Capabilities::LED);
}

bool HeadsetControl::hasInactiveTimeCapabilities() const
{
    return m_capabilities.contains(Capabilities::InactiveTime);
}

bool HeadsetControl::hasChatMixCapabilitiy() const
{
    return m_capabilities.contains(Capabilities::ChatMix);
}

bool HeadsetControl::hasVoicePromptCapabilitiy() const
{
    return m_capabilities.contains(Capabilities::VoicePrompt);
}

bool HeadsetControl::hasRotateToMuteCapabilitiy() const
{
    return m_capabilities.contains(Capabilities::RotateToMute);
}

bool HeadsetControl::hasEqualizerPresetCapability() const
{
    return m_capabilities.contains(Capabilities::EqualizerPreset);
}

void HeadsetControl::checkPath()
{
    m_queue.addProcess(m_path, {QStringLiteral("-h")});
    m_queue.start();
}

void HeadsetControl::queryAll()
{
    queryName();
    queryCapabilities();
}

void HeadsetControl::queryName()
{
    m_queue.addProcess(m_path, {});
    m_queue.start();
}

void HeadsetControl::queryBattery()
{
    m_queue.addProcess(m_path, {QStringLiteral("-cb")});
    m_queue.start();
}

void HeadsetControl::queryChatMix()
{
    m_queue.addProcess(m_path, {QStringLiteral("-cm")});
    m_queue.start();
}

void HeadsetControl::queryCapabilities()
{
    m_queue.addProcess(m_path, {QStringLiteral("-?c")});
    m_queue.start();
}

void HeadsetControl::setSidetone(int sidetone)
{
    m_queue.addProcess(m_path, {QStringLiteral("-s ").append(QString::number(sidetone))});
    m_queue.start();
}

void HeadsetControl::setNotificationSound(int sound)
{
    m_queue.addProcess(m_path, {QStringLiteral("-n ").append(QString::number(sound))});
    m_queue.start();
}

void HeadsetControl::setLed(bool enable)
{
    m_queue.addProcess(m_path, {QStringLiteral("-l ").append(enable ? QString::number(1) : QString::number(0))});
    m_queue.start();
}

void HeadsetControl::setInactiveTime(int time)
{
    m_queue.addProcess(m_path, {QStringLiteral("-i ").append(QString::number(time))});
    m_queue.start();
}

void HeadsetControl::setVoicePrompt(bool enable)
{
    m_queue.addProcess(m_path, {QStringLiteral("-v ").append(enable ? QString::number(1) : QString::number(0))});
    m_queue.start();
}

void HeadsetControl::setRotateToMute(bool enable)
{
    m_queue.addProcess(m_path, {QStringLiteral("-r ").append(enable ? QString::number(1) : QString::number(0))});
    m_queue.start();
}

void HeadsetControl::setEqualizerPreset(int preset)
{
    m_queue.addProcess(m_path, {QStringLiteral("-p ").append(QString::number(preset))});
    m_queue.start();
}

void HeadsetControl::outputReady(const QString &output, const QStringList &arguments)
{
    if (arguments.contains(QStringLiteral("-h"))) {
        if (output.contains(QStringLiteral("Headsetcontrol written by Sapd (Denis Arnst)")))
            setPathValid(true);
        else
            setPathValid(false);

        return;
    }

    if (arguments.isEmpty()) {
        if (isPathValid()) {
            auto nameString = output.trimmed().split(QStringLiteral("\n")).at(0);
            nameString.chop(1);
            setName(nameString.remove(QStringLiteral("Found ")));
        } else
            setName(QString());

        return;
    }

    if (arguments.contains(QStringLiteral("-?c"))) {
        if (isPathValid()) {
            QList<Capabilities> cap;
            if (output.contains(QStringLiteral("s")))
                cap.append(Capabilities::Sidetone);
            if (output.contains(QStringLiteral("b")))
                cap.append(Capabilities::Battery);
            if (output.contains(QStringLiteral("n")))
                cap.append(Capabilities::NotificationSound);
            if (output.contains(QStringLiteral("l")))
                cap.append(Capabilities::LED);
            if (output.contains(QStringLiteral("i")))
                cap.append(Capabilities::InactiveTime);
            if (output.contains(QStringLiteral("m")))
                cap.append(Capabilities::ChatMix);
            if (output.contains(QStringLiteral("v")))
                cap.append(Capabilities::VoicePrompt);
            if (output.contains(QStringLiteral("r")))
                cap.append(Capabilities::RotateToMute);
            if (output.contains(QStringLiteral("p")))
                cap.append(Capabilities::EqualizerPreset);
            setCapabilities(cap);
        } else
            setCapabilities(QList<Capabilities>());

        return;
    }

    if (arguments.contains(QStringLiteral("-cb"))) {
        if (isPathValid()) {
            bool check;
            auto batteryValue = output.toInt(&check);

            if (!check) {
                qWarning() << "Cannot parse battery info: " << output;
                setBattery(-2);
            } else
                setBattery(batteryValue);
        } else
            setBattery(-2);

        return;
    }

    if (arguments.contains(QStringLiteral("-cm"))) {
        if (isPathValid()) {
            bool check;
            auto chatMixValue = output.toInt(&check);

            if (!check) {
                qWarning() << "Cannot parse Chat-Mix info: " << output;
                setChatMix(-1);
            } else
                setChatMix(chatMixValue);
        } else
            setChatMix(-1);

        return;
    }

    if (arguments.contains(QStringLiteral("-s"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT sidetoneUpdated(true);
        else {
            qWarning() << "Failed to set sidetone: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT sidetoneUpdated(false);
        }

        return;
    }

    if (arguments.contains(QStringLiteral("-n"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT notificationSoundUpdated(true);
        else {
            qWarning() << "Failed to play notification sound: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT notificationSoundUpdated(false);
        }

        return;
    }

    if (arguments.contains(QStringLiteral("-l"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT ledUpdated(true);
        else {
            qWarning() << "Failed to set LED mode: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT ledUpdated(false);
        }

        return;
    }

    if (arguments.contains(QStringLiteral("-i"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT inactiveTimeUpdated(true);
        else {
            qWarning() << "Failed to set inactive time: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT inactiveTimeUpdated(false);
        }

        return;
    }

    if (arguments.contains(QStringLiteral("-v"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT voicePromptUpdated(true);
        else {
            qWarning() << "Failed to set voice prompt mode: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT voicePromptUpdated(false);
        }

        return;
    }

    if (arguments.contains(QStringLiteral("-r"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT rotateToMuteUpdated(true);
        else {
            qWarning() << "Failed to set rotate-to-mute mode: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT rotateToMuteUpdated(false);
        }

        return;
    }

    if (arguments.contains(QStringLiteral("-p"))) {
        if (output.contains(QStringLiteral("Success!")))
            Q_EMIT equalizerPresetUpdated(true);
        else {
            qWarning() << "Failed to set equalizer preset: " << output.split(QStringLiteral("\n")).at(1).simplified();
            Q_EMIT equalizerPresetUpdated(false);
        }

        return;
    }
}

void HeadsetControl::onPathChecked()
{
    if (isPathValid()) {
        queryName();
        queryCapabilities();
    }
}

void HeadsetControl::onCapabilitiesUpdated()
{
    if (hasBatteryCapability())
        queryBattery();
    if (hasChatMixCapabilitiy())
        queryChatMix();
}

void HeadsetControl::setPathValid(bool valid)
{
    if (m_pathValid != valid) {
        m_pathValid = valid;
        Q_EMIT pathChecked();
    }
}

void HeadsetControl::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        Q_EMIT nameChanged(m_name);
    }
    Q_EMIT nameQueried(m_name);
}

void HeadsetControl::setBattery(int battery)
{
    if (m_battery != battery) {
        m_battery = battery;
        Q_EMIT batteryChanged(m_battery);
    }
    Q_EMIT batteryQueried(m_battery);
}

void HeadsetControl::setChatMix(int chatMix)
{
    if (m_chatMix != chatMix) {
        m_chatMix = chatMix;
        Q_EMIT chatMixChanged(m_chatMix);
    }
    Q_EMIT chatMixQueried(m_chatMix);
}

void HeadsetControl::setCapabilities(const QList<Capabilities> &capabilities)
{
    if (m_capabilities != capabilities) {
        m_capabilities = capabilities;
        Q_EMIT capabilitiesChanged(m_capabilities);
    }
    Q_EMIT capabilitiesQueried(m_capabilities);
}
