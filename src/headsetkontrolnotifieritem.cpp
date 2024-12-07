#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>
#include <QTimer>

#include <KAboutData>
#include <KLocalizedString>

#include "headsetcontrol.h"
#include "headsetkontrolapplication.h"
#include "headsetkontrolconfig.h"
#include "headsetkontrolnotifieritem.h"

using namespace Qt::Literals::StringLiterals;

HeadsetKontrolNotifierItem::HeadsetKontrolNotifierItem(HeadsetKontrolApplication *parent)
    : KStatusNotifierItem{parent}
    , m_parent{parent}
{
    m_menu = new QMenu();

    setStandardActionsEnabled(false);
    setStatus(KStatusNotifierItem::Active);
    setContextMenu(m_menu);

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Help"));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::AboutApp));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::ReportBug));

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Actions"));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::HeadsetControlStart));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::HeadsetControlStop));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::HeadsetControlRefresh));

    m_deviceListMenu = new QMenu();
    m_deviceListMenu->setTitle(i18nc("@title:menu", "Show device"));
    m_menu->addMenu(m_deviceListMenu);

    m_devicePrimaryMenu = new QMenu();
    m_devicePrimaryMenu->setTitle(i18nc("@title:menu", "Select primary device"));
    m_menu->addMenu(m_devicePrimaryMenu);

    onDevicesChanged();
    connect(HeadsetControl::instance(), &HeadsetControl::devicesChanged, this, &HeadsetKontrolNotifierItem::onDevicesChanged);
    connect(HeadsetControl::instance(), &HeadsetControl::devicesChanged, this, &HeadsetKontrolNotifierItem::onDevicesChanged);

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Settings"));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::KeyBindings));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::Preferences));
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::ConfigureNotifications));

    m_menu->addSeparator();
    m_menu->addAction(m_parent->action(HeadsetKontrolApplication::Quit));

    connect(this, &HeadsetKontrolNotifierItem::activateRequested, parent, &HeadsetKontrolApplication::showWindow);

    connect(HeadsetControl::instance(), &HeadsetControl::devicesChanged, this, &HeadsetKontrolNotifierItem::updatePrimaryDevice);

    connect(HeadsetKontrolConfig::instance(), &HeadsetKontrolConfig::PrimaryChanged, this, &HeadsetKontrolNotifierItem::updatePrimaryDevice);

    setIconByName(u"headsetkontrol"_s);
}

HeadsetKontrolNotifierItem::~HeadsetKontrolNotifierItem()
{
}

void HeadsetKontrolNotifierItem::onDevicesChanged()
{
    m_deviceListMenu->clear();
    m_devicePrimaryMenu->clear();

    const auto devices = HeadsetControl::instance()->devices();
    for (const auto device : devices) {
        auto action = m_deviceListMenu->addAction(device->product().length() > 0 ? device->product() : device->device());
        connect(action, &QAction::triggered, device, [this, device]() {
            Q_EMIT m_parent->showDevice(device);
        });

        action = m_devicePrimaryMenu->addAction(device->product().length() > 0 ? device->product() : device->device());
        action->setCheckable(true);
        action->setChecked(device->setPrimaryAction()->isChecked());
        connect(action, &QAction::triggered, device->setPrimaryAction(), [this, device]() {
            device->setPrimaryAction()->trigger();
            this->setPrimaryDevice(device);
        });
    }
}

void HeadsetKontrolNotifierItem::updatePrimaryDevice()
{
    if (HeadsetControl::instance()->devices().length() <= 0)
        return setPrimaryDevice(nullptr);

    auto primary = HeadsetKontrolConfig::instance()->primary();
    if (primary.length() < 2) {
        return setPrimaryDevice(HeadsetControl::instance()->devices().at(0));
    }

    auto device = HeadsetControl::instance()->getDevice(primary.at(0), primary.at(1));
    setPrimaryDevice(device);
}

void HeadsetKontrolNotifierItem::update()
{
    QStringList subtitles;

    if (m_primaryDevice == nullptr) {
        setIconByName(u"headsetkontrol"_s);

        subtitles << i18nc("@info:tooltip", "No device found");
    } else {
        setIconByName(m_primaryDevice->battery()->batteryIconName());

        subtitles << i18nc("@info:tooltip", "Device: %1", m_primaryDevice->device());

        if (m_primaryDevice->battery()->status() == HeadsetControlDeviceBattery::Available
            || m_primaryDevice->battery()->status() == HeadsetControlDeviceBattery::Charging) {
            if (m_primaryDevice->battery()->level() >= 0) {
                subtitles << i18nc("@info:tooltip", "Battery: %1%", m_primaryDevice->battery()->level());
            } else {
                subtitles << i18nc("@info:tooltip", "Battery: Charging");
            }
        }

        if (m_primaryDevice->capabilities().testFlag(HeadsetControlDevice::ChatMix)) {
            subtitles << i18nc("@info:tooltip", "Chat-Mix: %1", m_primaryDevice->chatMix());
        }

        if (HeadsetControl::instance()->devices().length() > 1) {
            subtitles
                << (u"<i>%1</i>"_s)
                       .arg(i18ncp("@info:tooltip", "Found %1 other device", "Found %1 other devices", HeadsetControl::instance()->devices().length() - 1));
        }
    }

    setToolTipTitle(KAboutData::applicationData().displayName());
    setToolTipSubTitle(subtitles.join("<br>"_L1));
}

void HeadsetKontrolNotifierItem::setPrimaryDevice(HeadsetControlDevice *device)
{
    if (m_primaryDevice != nullptr)
        disconnect(m_primaryDevice->battery(), nullptr, this, nullptr);

    m_primaryDevice = device;

    if (m_primaryDevice != nullptr) {
        connect(m_primaryDevice->battery(), &HeadsetControlDeviceBattery::levelChanged, this, &HeadsetKontrolNotifierItem::update);
        connect(m_primaryDevice->battery(), &HeadsetControlDeviceBattery::statusChanged, this, &HeadsetKontrolNotifierItem::update);
        if (m_primaryDevice->capabilities().testFlag(HeadsetControlDevice::ChatMix)) {
            connect(m_primaryDevice, &HeadsetControlDevice::chatMixChanged, this, &HeadsetKontrolNotifierItem::update);
        }
    }

    update();
}
