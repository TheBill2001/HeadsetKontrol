// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadseterror.hpp"

#include "hkhcutils_p.hpp"
#include "hkmacros_p.hpp"

#include <KLazyLocalizedString>

using namespace Qt::StringLiterals;

HK_ASSERT_COPYABLE_MOVEABLE(HKHeadsetError)

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

    debug.nospace() << "HKHeadsetError(" << error.headset << ", " << HKUtils::enumKey<HKHeadset::Capabilities>(error.capability) << ", "
                    << HKUtils::enumKey(error.error);

    QString string = error.errorString;
    if (string.isEmpty()) {
        debug << ", " << string;
    }

    string = error.details;
    if (string.isEmpty()) {
        debug << ", " << string;
    }

    debug << ", " << error.timestamp << ")";

    return debug;
}

QString HKHeadsetError::localeErrorString() const
{
    if (errorString == QAnyStringView(timeErrorString.untranslatedText())) {
        return timeErrorString.toString();
    }
    if (errorString == QAnyStringView(deviceOfflineErrorString.untranslatedText())) {
        return deviceOfflineErrorString.toString();
    }
    if (errorString == QAnyStringView(protocolErrorString.untranslatedText())) {
        return protocolErrorString.toString();
    }
    if (errorString == QAnyStringView(invalidParameterErrorString.untranslatedText())) {
        return invalidParameterErrorString.toString();
    }
    if (errorString == QAnyStringView(notSupportedErrorString.untranslatedText())) {
        return notSupportedErrorString.toString();
    }
    if (errorString == QAnyStringView(permissionDeniedErrorString.untranslatedText())) {
        return permissionDeniedErrorString.toString();
    }
    if (errorString == QAnyStringView(usbErrorString.untranslatedText())) {
        return usbErrorString.toString();
    }
    if (errorString == QAnyStringView(hidErrorString.untranslatedText())) {
        return hidErrorString.toString();
    }
    if (errorString == QAnyStringView(outOfBoundsErrorString.untranslatedText())) {
        return outOfBoundsErrorString.toString();
    }
    if (errorString == QAnyStringView(unknownErrorString.untranslatedText())) {
        return unknownErrorString.toString();
    }
    return errorString;
}

QString HKHeadsetError::errorToLocaleString(Error error)
{
    switch (error) {
    case Timeout:
        return timeErrorString.toString();
    case DeviceOffline:
        return deviceOfflineErrorString.toString();
    case ProtocolError:
        return protocolErrorString.toString();
    case InvalidParameter:
        return invalidParameterErrorString.toString();
    case NotSupported:
        return notSupportedErrorString.toString();
    case PermissionDenied:
        return permissionDeniedErrorString.toString();
    case UsbError:
        return usbErrorString.toString();
    case HidError:
        return hidErrorString.toString();
    case OutOfBounds:
        return outOfBoundsErrorString.toString();
    default:
        break;
    }
    return unknownErrorString.toString();
}
