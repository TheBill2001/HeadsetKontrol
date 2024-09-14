#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QMenu>
#include <QTimer>

#include <KAboutData>
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>

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
    m_menu->addAction(m_parent->aboutAction());
    m_menu->addAction(m_parent->reportBugAction());

    m_menu->insertSection(nullptr, i18nc("@title:menu", "Actions"));
    m_menu->addAction(m_parent->startHeadsetControlAction());
    m_menu->addAction(m_parent->stopHeadsetControlAction());
    m_menu->addAction(m_parent->refreshHeadsetControlAction());

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
    m_menu->addAction(m_parent->configureKeyBindingsAction());
    m_menu->addAction(m_parent->configureAction());
    m_menu->addAction(m_parent->configureNotificationsAction());

    m_menu->addSeparator();
    m_menu->addAction(m_parent->quitAction());

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

void HeadsetKontrolNotifierItem::setPrimaryDevice(HeadsetControlDevice *device)
{
    if (m_primaryDevice != nullptr)
        disconnect(m_primaryDevice->battery(), nullptr, this, nullptr);

    m_primaryDevice = device;

    if (m_primaryDevice == nullptr)
        return setIconByName(u"headsetkontrol"_s);

    connect(m_primaryDevice->battery(), &HeadsetControlDeviceBattery::levelChanged, this, [this]() {
        setIconByName(m_primaryDevice->battery()->batteryIconName());
    });

    connect(m_primaryDevice->battery(), &HeadsetControlDeviceBattery::statusChanged, this, [this]() {
        setIconByName(m_primaryDevice->battery()->batteryIconName());
    });

    setIconByName(m_primaryDevice->battery()->batteryIconName());
}
