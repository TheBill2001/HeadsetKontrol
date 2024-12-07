#ifndef HEADSETCONTROLDEVICEBATTERY_H
#define HEADSETCONTROLDEVICEBATTERY_H

#include <QQmlEngine>

class HeadsetControlDevice;
class HeadsetControlDeviceBattery : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DeviceBattery)
    QML_UNCREATABLE("")

    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)
    Q_PROPERTY(int level READ level NOTIFY levelChanged FINAL)
public:
    enum Status {
        Available,
        Unavailable,
        Charging,
        HidError,
        Timeout
    };
    Q_ENUM(Status)

    explicit HeadsetControlDeviceBattery(HeadsetControlDevice *parent);
    ~HeadsetControlDeviceBattery();

    Status status() const;
    int level() const;

    void update(const QVariantHash &hash);
    QString batteryIconName() const;

Q_SIGNALS:
    void statusChanged();
    void levelChanged();

private:
    HeadsetControlDevice *m_device;
    Status m_status;
    int m_level;
    int m_lastNotifyLevel;

    void setStatus(const QString &newStatus);
    void setLevel(int newLevel);
    void notify();
};

#endif // HEADSETCONTROLDEVICEBATTERY_H
