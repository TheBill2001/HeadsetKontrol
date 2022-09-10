#ifndef HEADSETCONTROL_H
#define HEADSETCONTROL_H

#include <QList>
#include <QObject>

#include "processqueue.h"

class HeadsetControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(int battery READ getBattery NOTIFY batteryChanged)
    Q_PROPERTY(int chatMix READ getChatMix NOTIFY chatMixChanged)
    Q_PROPERTY(bool hasBatteryCapability READ hasBatteryCapability NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasSidetoneCapability READ hasSidetoneCapability NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasNotificationSoundCapability READ hasNotificationSoundCapability NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasLedCapability READ hasLedCapability NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasInactiveTimeCapabilities READ hasInactiveTimeCapabilities NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasChatMixCapabilitiy READ hasChatMixCapabilitiy NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasVoicePromptCapabilitiy READ hasVoicePromptCapabilitiy NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasRotateToMuteCapabilitiy READ hasRotateToMuteCapabilitiy NOTIFY capabilitiesChanged)
    Q_PROPERTY(bool hasEqualizerPresetCapability READ hasEqualizerPresetCapability NOTIFY capabilitiesChanged)
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    enum Capabilities {
        Sidetone = 0,
        Battery = 1,
        NotificationSound = 2,
        LED = 3,
        InactiveTime = 4,
        ChatMix = 5,
        VoicePrompt = 6,
        RotateToMute = 7,
        EqualizerPreset = 8
    };
    Q_ENUM(Capabilities)

    HeadsetControl(const QString &path = QString(), QObject *parent = nullptr);

    QString getName() const;
    int getBattery() const;
    int getChatMix() const;
    QList<Capabilities> getCapabilities() const;

    void setPath(const QString &path);
    QString getPath() const;
    bool isPathValid() const;

    bool hasBatteryCapability() const;
    bool hasSidetoneCapability() const;
    bool hasNotificationSoundCapability() const;
    bool hasLedCapability() const;
    bool hasInactiveTimeCapabilities() const;
    bool hasChatMixCapabilitiy() const;
    bool hasVoicePromptCapabilitiy() const;
    bool hasRotateToMuteCapabilitiy() const;
    bool hasEqualizerPresetCapability() const;

public Q_SLOTS:
    Q_INVOKABLE void checkPath();
    Q_INVOKABLE void queryAll();
    Q_INVOKABLE void queryName();
    Q_INVOKABLE void queryBattery();
    Q_INVOKABLE void queryChatMix();
    Q_INVOKABLE void queryCapabilities();

    Q_INVOKABLE void setSidetone(int sidetone);
    Q_INVOKABLE void setNotificationSound(int sound);
    Q_INVOKABLE void setLed(bool enable);
    Q_INVOKABLE void setInactiveTime(int time);
    Q_INVOKABLE void setVoicePrompt(bool enable);
    Q_INVOKABLE void setRotateToMute(bool enable);
    Q_INVOKABLE void setEqualizerPreset(int preset);

private Q_SLOTS:
    void outputReady(const QString &output, const QStringList &arguments);
    void onCapabilitiesQuerried();
    void onNameChanged();

Q_SIGNALS:
    void nameQueried(const QString &name);
    void batteryQueried(int battery);
    void chatMixQueried(int chatMix);
    void capabilitiesQueried(const QList<HeadsetControl::Capabilities> &capabilities);
    void pathChecked();

    void nameChanged(const QString &name);
    void batteryChanged(int battery);
    void chatMixChanged(int chatMix);
    void capabilitiesChanged(const QList<HeadsetControl::Capabilities> &capabilities);
    void pathChanged(const QString &path);

    void sidetoneUpdated(bool success);
    void notificationSoundUpdated(bool success);
    void ledUpdated(bool success);
    void inactiveTimeUpdated(bool success);
    void voicePromptUpdated(bool success);
    void rotateToMuteUpdated(bool success);
    void equalizerPresetUpdated(bool success);

private:
    QString m_path;
    QString m_name;
    int m_battery;
    int m_chatMix;
    QList<Capabilities> m_capabilities;
    bool m_pathValid;
    ProcessQueue m_queue;

    void setPathValid(bool valid);
    void setName(const QString &name);
    void setBattery(int battery);
    void setChatMix(int chatMix);
    void setCapabilities(const QList<Capabilities> &capabilities);
    void enqueue(const QStringList &arguments);
};

#endif
