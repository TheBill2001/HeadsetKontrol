#ifndef HEADSETKONTROLNOTIFIERITEM_H
#define HEADSETKONTROLNOTIFIERITEM_H

#include <QQmlEngine>

#include <KStatusNotifierItem>

class KActionCollection;

class HeadsetControlDevice;
class HeadsetKontrolApplication;
class HeadsetKontrolNotifierItem : public KStatusNotifierItem
{
    Q_OBJECT
public:
    HeadsetKontrolNotifierItem(HeadsetKontrolApplication *parent = nullptr);
    ~HeadsetKontrolNotifierItem();

private Q_SLOTS:
    void onDevicesChanged();
    void updatePrimaryDevice();
    void update();

private:
    HeadsetKontrolApplication *m_parent;
    HeadsetControlDevice *m_primaryDevice = nullptr;

    QMenu *m_menu;
    QMenu *m_deviceListMenu;
    QMenu *m_devicePrimaryMenu;

    void setPrimaryDevice(HeadsetControlDevice *device);
};

#endif // HEADSETKONTROLNOTIFIERITEM_H
