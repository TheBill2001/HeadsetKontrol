#include "headsetcontroldevicebattery.h"

#include <KLocalizedString>
#include <KNotification>

#include "headsetcontroldevice.h"
#include "headsetkontrolconfig.h"

using namespace Qt::Literals::StringLiterals;

HeadsetControlDeviceBattery::HeadsetControlDeviceBattery(HeadsetControlDevice *parent)
    : QObject{parent}
    , m_device{parent}
    , m_level{-1}
    , m_oldLevel{-1}
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

    bool notify = false;
    if (abs(m_level - m_oldLevel) > 10) {
        notify = true;
        m_oldLevel = m_level;
    }

    m_level = newLevel;
    Q_EMIT levelChanged();

    if (!(HeadsetKontrolConfig::instance()->batteryNotification() && notify)) {
        return;
    }

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

    if (level() < 0) {
        notification->setIconName(u"headsetkontrol"_s);
    } else if (level() < 10) {
        notification->setIconName(u"headsetkontrol_battery_empty"_s);
    } else if (level() < 35) {
        notification->setIconName(u"headsetkontrol_battery_low"_s);
    } else if (level() < 65) {
        notification->setIconName(u"headsetkontrol_battery_medium"_s);
    } else if (level() < 90) {
        notification->setIconName(u"headsetkontrol_battery_high"_s);
    } else {
        notification->setIconName(u"headsetkontrol_battery_full"_s);
    }

    notification->sendEvent();
}
