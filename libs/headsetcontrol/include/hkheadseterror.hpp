// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETERROR_HPP
#define HKHEADSETERROR_HPP

#include "hkheadset.hpp"
#include "hkheadsetid.hpp"

#include <QDateTime>
#include <QPointer>

class HKHC_EXPORT HKHeadsetError
{
    Q_GADGET
    Q_PROPERTY(HKHeadset *headset READ headset CONSTANT FINAL)
    Q_PROPERTY(HKHeadsetId headsetId READ headsetId CONSTANT FINAL)
    Q_PROPERTY(QString headsetName READ headsetName CONSTANT FINAL)
    Q_PROPERTY(HKHeadset::Capability capability READ capability CONSTANT FINAL)
    Q_PROPERTY(HKHeadsetError::Error error READ error CONSTANT FINAL)
    Q_PROPERTY(QString errorString READ errorString CONSTANT FINAL)
    Q_PROPERTY(QString details READ details CONSTANT FINAL)
    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT FINAL)
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

    HKHeadsetError() noexcept = default;

    HKHeadsetError(HKHeadset *headset,
                   HKHeadsetId headsetId,
                   QAnyStringView headsetName,
                   HKHeadset::Capability capability,
                   Error error,
                   QAnyStringView errorString,
                   QAnyStringView details,
                   QDateTime timestamp) noexcept
        : m_headset{headset}
        , m_headsetId{headsetId}
        , m_headsetName{headsetName.toString()}
        , m_capability{capability}
        , m_error{error}
        , m_errorString{errorString.toString()}
        , m_details{details.toString()}
        , m_timestamp{std::move(timestamp)}
    {
    }

    [[nodiscard]] HKHeadset *headset() const noexcept
    {
        return m_headset;
    }

    [[nodiscard]] HKHeadsetId headsetId() const noexcept
    {
        return m_headsetId;
    }

    [[nodiscard]] QString headsetName() const noexcept
    {
        return m_headsetName;
    }

    [[nodiscard]] HKHeadset::Capability capability() const noexcept
    {
        return m_capability;
    }

    [[nodiscard]] Error error() const noexcept
    {
        return m_error;
    }

    [[nodiscard]] QString errorString() const noexcept
    {
        return m_errorString;
    }

    [[nodiscard]] QString details() const noexcept
    {
        return m_details;
    }

    [[nodiscard]] QDateTime timestamp() const noexcept
    {
        return m_timestamp;
    }

    friend bool operator==(const HKHeadsetError &lhs, const HKHeadsetError &rhs);
    friend QDebug operator<<(QDebug debug, const HKHeadsetError &error);

private:
    QPointer<HKHeadset> m_headset;
    HKHeadsetId m_headsetId;
    QString m_headsetName;
    HKHeadset::Capability m_capability{HKHeadset::UnknownCapability};
    Error m_error{Unknown};
    QString m_errorString;
    QString m_details;
    QDateTime m_timestamp;
};

Q_DECLARE_TYPEINFO(HKHeadsetError, Q_RELOCATABLE_TYPE);

[[nodiscard]] inline bool operator==(const HKHeadsetError &lhs, const HKHeadsetError &rhs) = default;

[[maybe_unused]] HKHC_EXPORT QDebug operator<<(QDebug debug, const HKHeadsetError &error);

#endif // HKHEADSETERROR_HPP
