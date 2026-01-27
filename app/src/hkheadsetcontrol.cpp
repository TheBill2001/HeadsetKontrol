// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadsetcontrol.hpp"

#include "hkheadset_p.hpp"
#include "hkheadsetcontrol_p.hpp"
#include "hkheadsetid.hpp"
#include "hklogging.hpp"

#include <headsetcontrol.hpp>

#include <QApplicationStatic>
#include <QCollator>
#include <QQmlEngine>
#include <QThreadPool>

#include <KLocalizedString>

HKHeadsetControlPrivate::HKHeadsetControlPrivate(HKHeadsetControl *const q_ptr)
    : HKObjectPrivate{q_ptr}
    , version{[this] {
        QMutexLocker lock(&internalMutex);
        return STD_STRVIEW_TO_QSTR(headsetcontrol::version());
    }()}
    , supportedDevices{[this] {
        QMutexLocker lock(&internalMutex);
        const auto internalSupportedDevices = headsetcontrol::supportedDevices();
        QStringList supportedDevices;
        for (std::string_view headsetName : internalSupportedDevices) {
            supportedDevices << QAnyStringView(headsetName).toString();
        }
        QCollator collator(QLocale::English);
        std::ranges::sort(supportedDevices, collator);
        return supportedDevices;
    }()}
    , deviceTimeout{500}
    , testDeviceEnabled{false}
    , testProfile{0}
    , running{false}
{
    QObject::connect(q_ptr, &HKHeadsetControl::updateRateChanged, q_ptr, [this](int value) {
        timer.setInterval(value);

        if (timer.isActive()) {
            refresh();
        }
    });

    QObject::connect(q_ptr, &HKHeadsetControl::deviceTimeoutChanged, q_ptr, [this](int value) {
        {
            QMutexLocker lock(&internalMutex);
            headsetcontrol::setDeviceTimeout(value);
        }

        if (timer.isActive()) {
            refresh();
        }
    });

    QObject::connect(q_ptr, &HKHeadsetControl::testDeviceEnabledChanged, q_ptr, [this](bool value) {
        {
            QMutexLocker lock(&internalMutex);
            headsetcontrol::enableTestDevice(value);
        }

        if (timer.isActive()) {
            refresh();
        }
    });

    QObject::connect(q_ptr, &HKHeadsetControl::testProfileChanged, q_ptr, [this](int value) {
        {
            QMutexLocker lock(&internalMutex);
            headsetcontrol::setTestProfile(value);
        }

        if (timer.isActive()) {
            refresh();
        }
    });

    {
        QMutexLocker lock(&internalMutex);
        headsetcontrol::setDeviceTimeout(deviceTimeout.value());
        headsetcontrol::enableTestDevice(testDeviceEnabled.value());
        headsetcontrol::setTestProfile(testProfile.value());
    }

    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(std::chrono::milliseconds{updateRate.value()});

    QObject::connect(&timer, &QTimer::timeout, q_ptr, [this] {
        refresh();
    });
}

HKHeadsetControlPrivate::~HKHeadsetControlPrivate()
{
    // Explicitly delete
    qDeleteAll(headsets.value());
}

void HKHeadsetControlPrivate::refresh()
{
    if (updating) {
        return;
    }
    updating = true;

    QThreadPool::globalInstance()->start([this, oldHeadsets = headsets.value(), discoverAll = discoverAll.value()]() mutable {
        auto discoveredHeadsets = discoverAll ? headsetcontrol::discoverAll() : headsetcontrol::discover();

        QList<HKHeadset *> toDelete;
        QList<HKHeadset *> toUpdate;
        QList<HKHeadset *> toAdded;

        for (auto oldHeadsetIter = oldHeadsets.begin(); oldHeadsetIter != oldHeadsets.end();) {
            const auto discoveredHeadsetIter = std::ranges::find_if(discoveredHeadsets, [&oldHeadsetIter](const headsetcontrol::Headset &internalHeadset) {
                return HKHeadsetId{internalHeadset.vendorId(), internalHeadset.productId()} == (*oldHeadsetIter)->id();
            });

            if (discoveredHeadsetIter == discoveredHeadsets.end()) {
                qCInfo(HK_LOGGING, "Headset removed: %s (%s).", qPrintable((*oldHeadsetIter)->name()), qPrintable((*oldHeadsetIter)->id()));
                toDelete << *oldHeadsetIter;
                oldHeadsetIter = oldHeadsets.erase(oldHeadsetIter);
            } else {
                (*oldHeadsetIter)->d_ptr->internalHeadset = std::move(*discoveredHeadsetIter);
                toUpdate << *oldHeadsetIter;
                discoveredHeadsets.erase(discoveredHeadsetIter);
                ++oldHeadsetIter;
            }
        }

        for (auto &discoveredHeadset : discoveredHeadsets) {
            auto *newHeadset = new HKHeadset(HKHeadset::InitData{.internalMutex = internalMutex, .internalHeadset = std::move(discoveredHeadset)});
            newHeadset->moveToThread(q_ptr->thread());
            newHeadset->setParent(q_ptr);
            newHeadset->d_ptr->refresh(true);
            toAdded << newHeadset;
            qCInfo(HK_LOGGING, "Headset added: %s (%s).", qPrintable(newHeadset->name()), qPrintable(newHeadset->id()));
        }
        discoveredHeadsets.clear();

        for (auto *headset : std::as_const(toUpdate)) {
            qCDebug(HK_LOGGING, "Headset updating: %s (%s).", qPrintable(headset->name()), qPrintable(headset->id()));
            headset->d_ptr->refresh(true);
        }

        q_ptr->metaObject()->invokeMethod(q_ptr, [this, toUpdate = std::move(toUpdate), toAdded = std::move(toAdded), toDelete = std::move(toDelete)] {
            headsets = toUpdate + toAdded;

            Q_Q(HKHeadsetControl);
            if (!toAdded.isEmpty()) {
                Q_EMIT q->headsetsAdded(toAdded, HKHeadsetControl::QPrivateSignal{});
            }

            if (!toDelete.isEmpty()) {
                Q_EMIT q->headsetsRemoved(toDelete, HKHeadsetControl::QPrivateSignal{});
                for (auto *headset : std::as_const(toDelete)) {
                    headset->deleteLater();
                }
            }

            updating = false;
            if (!timer.isActive()) {
                running = false;
            }
        });
    });
}

class HK_NO_EXPORT HKHeadsetControlStatic
{
    Q_DISABLE_COPY_MOVE(HKHeadsetControlStatic)
public:
    HKHeadsetControl instance;

    HKHeadsetControlStatic()
    {
        instance.moveToThread(qApp->thread());
    }
};
Q_APPLICATION_STATIC(HKHeadsetControlStatic, g_static) // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

HKHeadsetControl::HKHeadsetControl()
    : d_ptr{new HKHeadsetControlPrivate(this)}
{
}

HKHeadsetControl::~HKHeadsetControl()
{
    delete d_ptr;
}

HKHeadsetControl *HKHeadsetControl::instance()
{
    return &g_static->instance;
}

HKHeadsetControl *HKHeadsetControl::create(QQmlEngine *engine, QJSEngine * /*unused*/)
{
    auto *const instance = HKHeadsetControl::instance();
    Q_ASSERT(instance->thread() == engine->thread());
    QQmlEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

QString HKHeadsetControl::version() noexcept
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->version;
}

QStringList HKHeadsetControl::supportedDevices() noexcept
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->supportedDevices;
}

QBindable<int> HKHeadsetControl::bindableUpdateRate()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return &d->updateRate;
}

int HKHeadsetControl::updateRate()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return d->updateRate.value();
}

void HKHeadsetControl::setUpdateRate(int value)
{
    Q_D_SINGLETON(HKHeadsetControl);
    d->updateRate = value;
}

QBindable<int> HKHeadsetControl::bindableDeviceTimeout()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return &d->deviceTimeout;
}

int HKHeadsetControl::deviceTimeout()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->deviceTimeout.value();
}

void HKHeadsetControl::setDeviceTimeout(int value)
{
    Q_D_SINGLETON(HKHeadsetControl);
    d->deviceTimeout = value;
}

QBindable<bool> HKHeadsetControl::bindableTestDeviceEnabled()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return &d->testDeviceEnabled;
}

bool HKHeadsetControl::testDeviceEnabled()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->testDeviceEnabled.value();
}

void HKHeadsetControl::setTestDeviceEnabled(bool value)
{
    Q_D_SINGLETON(HKHeadsetControl);
    d->testDeviceEnabled = value;
}

QBindable<int> HKHeadsetControl::bindableTestProfile()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return &d->testProfile;
}

int HKHeadsetControl::testProfile()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->testProfile.value();
}

void HKHeadsetControl::setTestProfile(int value)
{
    Q_D_SINGLETON(HKHeadsetControl);
    d->testProfile = value;
}

QBindable<bool> HKHeadsetControl::bindableDiscoverAll()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return &d->discoverAll;
}

bool HKHeadsetControl::discoverAll()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->discoverAll.value();
}

void HKHeadsetControl::setDiscoverAll(bool value)
{
    Q_D_SINGLETON(HKHeadsetControl);
    d->discoverAll = value;
}

QBindable<bool> HKHeadsetControl::bindableRunning()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return &d->running;
}

bool HKHeadsetControl::isRunning()
{
    Q_D_SINGLETON(HKHeadsetControl);
    return d->running.value();
}

int HKHeadsetControl::timeUntilRefresh()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->timer.remainingTime();
}

QBindable<QList<HKHeadset *>> HKHeadsetControl::bindableHeadsets()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return &d->headsets;
}

QList<HKHeadset *> HKHeadsetControl::headsets()
{
    Q_D_SINGLETON(const HKHeadsetControl);
    return d->headsets.value();
}

void HKHeadsetControl::start()
{
    HKHeadsetControl::staticMetaObject.invokeMethod(instance(), []() {
        Q_D_SINGLETON(HKHeadsetControl);
        d->running = true;
        d->timer.start();
        d->refresh();
    });
}

void HKHeadsetControl::stop()
{
    HKHeadsetControl::staticMetaObject.invokeMethod(instance(), []() {
        Q_D_SINGLETON(HKHeadsetControl);
        d->timer.stop();
        d->running = false;
    });
}

void HKHeadsetControl::refresh()
{
    HKHeadsetControl::staticMetaObject.invokeMethod(instance(), []() {
        Q_D_SINGLETON(HKHeadsetControl);
        if (d->running.value()) {
            d->timer.start();
        }
        d->running = true;
        d->refresh();
    });
}
