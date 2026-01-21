// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETCONTROL_HPP
#define HKHEADSETCONTROL_HPP

#include <QObject>

class HKHeadset;
class HKHeadsetControlPrivate;
class HKHeadsetControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version CONSTANT FINAL)
    Q_PROPERTY(QStringList supportedDevices READ supportedDevices CONSTANT FINAL)
    Q_PROPERTY(int deviceTimeout READ deviceTimeout WRITE setTestDeviceEnabled NOTIFY deviceTimeoutChanged BINDABLE bindableDeviceTimeout FINAL)
    Q_PROPERTY(
        bool testDeviceEnabled READ testDeviceEnabled WRITE setTestDeviceEnabled NOTIFY testDeviceEnabledChanged BINDABLE bindableTestDeviceEnabled FINAL)
    Q_PROPERTY(int testProfile READ testProfile WRITE setTestProfile NOTIFY testProfileChanged BINDABLE bindableTestProfile FINAL)
    Q_PROPERTY(bool discoverAll READ discoverAll WRITE setDiscoverAll NOTIFY discoverAllChanged BINDABLE bindableDiscoverAll FINAL)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged BINDABLE bindableRunning FINAL)
    Q_PROPERTY(QList<HKHeadset *> headsets READ headsets NOTIFY headsetsChanged BINDABLE bindableHeadsets)

    Q_DECLARE_PRIVATE(HKHeadsetControl)
    HKHeadsetControlPrivate *const d_ptr;

    friend class HKHeadsetControlStatic;
    HKHeadsetControl();

public:
    ~HKHeadsetControl() override;

    [[nodiscard]] static HKHeadsetControl *instance();

    [[nodiscard]] static QString version() noexcept;
    [[nodiscard]] static QStringList supportedDevices() noexcept;

    [[nodiscard]] static QBindable<int> bindableUpdateRate();
    [[nodiscard]] static int updateRate();
    static void setUpdateRate(int value);

    [[nodiscard]] static QBindable<int> bindableDeviceTimeout();
    [[nodiscard]] static int deviceTimeout();
    static void setDeviceTimeout(int value);

    [[nodiscard]] static QBindable<bool> bindableTestDeviceEnabled();
    [[nodiscard]] static bool testDeviceEnabled();
    static void setTestDeviceEnabled(bool value);

    [[nodiscard]] static QBindable<int> bindableTestProfile();
    [[nodiscard]] static int testProfile();
    static void setTestProfile(int value);

    [[nodiscard]] static QBindable<bool> bindableDiscoverAll();
    [[nodiscard]] static bool discoverAll();
    static void setDiscoverAll(bool value);

    [[nodiscard]] static QBindable<bool> bindableRunning();
    [[nodiscard]] static bool running();

    [[nodiscard]] static QBindable<QList<HKHeadset *>> bindableHeadsets();
    [[nodiscard]] static QList<HKHeadset *> headsets();

public Q_SLOTS:
    static void start();
    static void stop();
    static void refresh();

Q_SIGNALS:
    void updateRateChanged(int, QPrivateSignal);
    void deviceTimeoutChanged(int, QPrivateSignal);
    void testDeviceEnabledChanged(bool, QPrivateSignal);
    void testProfileChanged(int, QPrivateSignal);
    void discoverAllChanged(bool, QPrivateSignal);
    void runningChanged(bool, QPrivateSignal);
    void headsetsChanged(const QList<HKHeadset *> &, QPrivateSignal);
    void headsetsAdded(const QList<HKHeadset *> &, QPrivateSignal);
    void headsetsRemoved(const QList<HKHeadset *> &, QPrivateSignal);
};

Q_MOC_INCLUDE("hkheadset.hpp")

#endif // HKHEADSETCONTROL_HPP
