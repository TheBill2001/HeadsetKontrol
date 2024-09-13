#ifndef HEADSETCONTROL_H
#define HEADSETCONTROL_H

#include <QQmlEngine>
#include <QTimer>

#include "headsetcontroldevice.h"
#include "processqueue.h"

class HeadsetControl : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString version READ version NOTIFY versionChanged FINAL)
    Q_PROPERTY(QString apiVersion READ apiVersion NOTIFY apiVersionChanged FINAL)
    Q_PROPERTY(QString hidApiVersion READ hidApiVersion NOTIFY hidApiVersionChanged FINAL)
    Q_PROPERTY(int updateRate READ updateRate WRITE setUpdateRate NOTIFY updateRateChanged FINAL)
    Q_PROPERTY(QList<HeadsetControlDevice *> devices READ devices NOTIFY devicesChanged FINAL)
    Q_PROPERTY(qint32 countDownTime READ countDownTime NOTIFY countDownTimeChanged FINAL)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged FINAL)
public:
    explicit HeadsetControl(QObject *parent);

    static HeadsetControl *create(QQmlEngine *, QJSEngine *);
    static HeadsetControl *instance();

    QString version() const;
    QString apiVersion() const;
    QString hidApiVersion() const;

    int updateRate() const;
    void setUpdateRate(int newUpdateRate);

    QList<HeadsetControlDevice *> devices() const;

    qint32 countDownTime() const;
    bool isRunning() const;

public Q_SLOTS:
    void start();
    void stop();
    void refresh();
    void queue(const QString &arg, const QVariant &value);

    void stopCountDownUpdateTimer();
    void startCountDownUpdateTimer();

Q_SIGNALS:
    void versionChanged();
    void apiVersionChanged();
    void hidApiVersionChanged();
    void updateRateChanged();
    void devicesChanged();
    void parsingErrorOccurred(const QString &error);
    void processErrorOccurred(const QString &error);
    void deviceErrorOccurred(HeadsetControlDevice *device, const QString &error);
    void countDownTimeChanged();
    void isRunningChanged();

private Q_SLOTS:
    void onUpdated(const QByteArray &data);
    void run();

private:
    ProcessQueue *m_queue;

    QString m_version;
    QString m_apiVersion;
    QString m_hidApiVersion;

    qint32 m_updateRate;

    QTimer m_timer;
    QTimer m_countdownRefreshTimer;
    QTimer m_queueDelayTimer;

    QVariantHash m_delayedQueue;

    QList<HeadsetControlDevice *> m_devices;

    void setIsPathValid(bool newIsPathValid);
    void setVersion(const QString &newVersion);
    void setApiVersion(const QString &newApiVersion);
    void setHidApiVersion(const QString &newHidApiVersion);

    void addDevice(HeadsetControlDevice *device);
    void removeDevice(HeadsetControlDevice *device);
    void clearDevices();
    HeadsetControlDevice *getDevice(const QString &vendorId, const QString &productId);
};

#endif
