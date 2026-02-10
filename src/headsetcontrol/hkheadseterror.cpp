// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadseterror.hpp"

#include "hkutils.hpp"

#include <KLazyLocalizedString>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto timeErrorString = kli18nc("headset error string", "Operation timed out");
constexpr auto deviceOfflineErrorString = kli18nc("headset error string", "Device is offline or not responding");
constexpr auto protocolErrorString = kli18nc("headset error string", "Protocol error");
constexpr auto invalidParameterErrorString = kli18nc("headset error string", "Invalid parameter");
constexpr auto notSupportedErrorString = kli18nc("headset error string", "Feature not supported by this device");
constexpr auto permissionDeniedErrorString = kli18nc("headset error string", "Permission denied");
constexpr auto usbErrorString = kli18nc("headset error string", "USB communication error");
constexpr auto hidErrorString = kli18nc("headset error string", "HID communication error");
constexpr auto outOfBoundsErrorString = kli18nc("headset error string", "Out of bounds");
constexpr auto unknownErrorString = kli18nc("headset error string", "Unknown error");
}

QDebug operator<<(QDebug debug, const HKHeadsetError &error)
{
    QDebugStateSaver saver(debug);

    debug.nospace() << "HKHeadsetError(" << error.m_headset << ", " << HKUtils::flagKey(error.m_capability) << ", " << HKUtils::enumKey(error.m_error);

    if (error.m_errorString.isEmpty()) {
        debug << ", " << error.m_errorString;
    }

    if (error.m_details.isEmpty()) {
        debug << ", " << error.m_details;
    }

    debug << ", " << error.m_timestamp << ")";

    return debug;
}

namespace HKUtils
{
QString headsetErrorLocaleErrorString(const HKHeadsetError &error)
{
    if (error.errorString() == QAnyStringView(timeErrorString.untranslatedText())) {
        return timeErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(deviceOfflineErrorString.untranslatedText())) {
        return deviceOfflineErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(protocolErrorString.untranslatedText())) {
        return protocolErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(invalidParameterErrorString.untranslatedText())) {
        return invalidParameterErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(notSupportedErrorString.untranslatedText())) {
        return notSupportedErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(permissionDeniedErrorString.untranslatedText())) {
        return permissionDeniedErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(usbErrorString.untranslatedText())) {
        return usbErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(hidErrorString.untranslatedText())) {
        return hidErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(outOfBoundsErrorString.untranslatedText())) {
        return outOfBoundsErrorString.toString();
    }
    if (error.errorString() == QAnyStringView(unknownErrorString.untranslatedText())) {
        return unknownErrorString.toString();
    }
    return error.errorString();
}

QString headsetErrorToLocaleString(HKHeadsetError::Error error)
{
    switch (error) {
    case HKHeadsetError::Timeout:
        return timeErrorString.toString();
    case HKHeadsetError::DeviceOffline:
        return deviceOfflineErrorString.toString();
    case HKHeadsetError::ProtocolError:
        return protocolErrorString.toString();
    case HKHeadsetError::InvalidParameter:
        return invalidParameterErrorString.toString();
    case HKHeadsetError::NotSupported:
        return notSupportedErrorString.toString();
    case HKHeadsetError::PermissionDenied:
        return permissionDeniedErrorString.toString();
    case HKHeadsetError::UsbError:
        return usbErrorString.toString();
    case HKHeadsetError::HidError:
        return hidErrorString.toString();
    case HKHeadsetError::OutOfBounds:
        return outOfBoundsErrorString.toString();
    default:
        break;
    }
    return unknownErrorString.toString();
}
} // namespace HKUtils
