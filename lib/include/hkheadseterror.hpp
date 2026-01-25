// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETERROR_HPP
#define HKHEADSETERROR_HPP

#include "hkheadset.hpp"
#include "hkheadsetid.hpp"

#include <QDateTime>
#include <QPointer>

class HK_EXPORT HKHeadsetError
{
    Q_GADGET
    Q_PROPERTY(HKHeadset *headset MEMBER headset FINAL)
    Q_PROPERTY(HKHeadsetId headsetId MEMBER headsetId FINAL)
    Q_PROPERTY(QString headsetName MEMBER headsetName FINAL)
    Q_PROPERTY(HKHeadset::Capability capability MEMBER capability FINAL)
    Q_PROPERTY(HKHeadsetError::Error error MEMBER error FINAL)
    Q_PROPERTY(QString errorString MEMBER errorString FINAL)
    Q_PROPERTY(QString details MEMBER details FINAL)
    Q_PROPERTY(QDateTime timestamp MEMBER timestamp FINAL)
public:
    enum Error : quint8 {
        Unknown = 0,
        Timeout,
        DeviceOffline,
        ProtocolError,
        InvalidParameter,
        NotSupported,
        PermissionDenied,
        UsbError,
        HidError,
        OutOfBounds
    };
    Q_ENUM(Error)

    QPointer<HKHeadset> headset;
    HKHeadsetId headsetId;
    QString headsetName;
    HKHeadset::Capability capability;
    Error error;
    QString errorString;
    QString details;
    QDateTime timestamp;

    friend bool operator==(const HKHeadsetError &lhs, const HKHeadsetError &rhs);
};

[[nodiscard]] inline bool operator==(const HKHeadsetError &lhs, const HKHeadsetError &rhs) = default;

[[maybe_unused]] HK_EXPORT QDebug operator<<(QDebug debug, const HKHeadsetError &error);

Q_DECLARE_TYPEINFO(HKHeadsetError, Q_RELOCATABLE_TYPE);

#endif // HKHEADSETERROR_HPP
