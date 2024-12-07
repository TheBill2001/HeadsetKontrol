#include "headsetcontroldevicebattery.h"

#include <KLocalizedString>
#include <KNotification>

#include "headsetcontroldevice.h"

using namespace Qt::Literals::StringLiterals;

HeadsetControlDeviceBattery::HeadsetControlDeviceBattery(HeadsetControlDevice *parent)
    : QObject{parent}
    , m_device{parent}
    , m_level{-1}
    , m_lastNotifyLevel{-1}
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

QString HeadsetControlDeviceBattery::batteryIconName() const
{
    if (this->level() < 0 || !(status() == Charging || status() == Available)) {
        return u"headsetkontrol"_s;
    }

    auto level = 10 * ((this->level() + 5) / 10);
    if (status() == Charging) {
        return u"headsetkontrol-battery-%1-charging"_s.arg(level, 3, 10, '0'_L1);
    } else {
        return u"headsetkontrol-battery-%1"_s.arg(level, 3, 10, '0'_L1);
    }
}

void HeadsetControlDeviceBattery::setLevel(int newLevel)
{
    if (m_level == newLevel)
        return;

    m_level = newLevel;
    Q_EMIT levelChanged();

    auto shouldNotify = abs(m_level - m_lastNotifyLevel) >= 10;

    if (m_level <= 0 || m_level >= 100) {
        if (m_lastNotifyLevel != m_level) {
            shouldNotify = true;
        }
    }

    if (shouldNotify) {
        m_lastNotifyLevel = m_level;
        notify();
    }
}

void HeadsetControlDeviceBattery::notify()
{
    auto notification = new KNotification(QStringLiteral("batteryStatus"), KNotification::CloseOnTimeout, this);
    notification->setAutoDelete(true);
    notification->setTitle(m_device->device());

    if (status() == Charging) {
        if (level() == -1) {
            notification->setText(i18nc("@info:status", "Battery is charging."));
        } else {
            notification->setText(i18nc("@info:status", "Battery is charging at %1%.", level()));
        }
    } else {
        notification->setText(i18nc("@info:status", "Battery is at %1%.", level()));
    }

    notification->setIconName(batteryIconName());
    notification->sendEvent();
}