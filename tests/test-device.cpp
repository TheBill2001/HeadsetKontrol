// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkbattery.hpp"
#include "hkchatmix.hpp"
#include "hkheadset.hpp"
#include "hkheadsetcontrol.hpp"
#include "hkheadseterror.hpp"
#include "hkheadsetid.hpp"

#include <QSignalSpy>
#include <QTest>

#define COMPARE_RESULT(ResultType, Data, Capability, Type)                                                                                                     \
    {                                                                                                                                                          \
        QFETCH(ResultType, Data);                                                                                                                              \
        if (std::holds_alternative<Type>(Data)) {                                                                                                              \
            QCOMPARE(headset->Data(), std::get<Type>(Data));                                                                                                   \
        } else {                                                                                                                                               \
            auto error = std::ranges::find_if(errors, [](const HKHeadsetError &error) {                                                                        \
                return error.capability == Capability;                                                                                                         \
            });                                                                                                                                                \
            QCOMPARE_NE(error, errors.constEnd());                                                                                                             \
            const auto &expectedError = std::get<HKHeadsetError>(Data);                                                                                        \
            QCOMPARE(error->capability, expectedError.capability);                                                                                             \
            QCOMPARE(error->error, expectedError.error);                                                                                                       \
            QCOMPARE(error->errorString, expectedError.errorString);                                                                                           \
            QCOMPARE(error->details, expectedError.details);                                                                                                   \
        }                                                                                                                                                      \
    }

using namespace Qt::StringLiterals;

using BatteryResult = std::variant<HKBattery, HKHeadsetError>;
Q_DECLARE_METATYPE(BatteryResult)

using ChatMixResult = std::variant<HKChatMix, HKHeadsetError>;
Q_DECLARE_METATYPE(ChatMixResult)

namespace
{
template<typename Enum, typename Flags = QFlags<Enum>>
Flags allFlags()
{
    const auto metaEnum = QMetaEnum::fromType<Flags>();
    Q_ASSERT(metaEnum.isFlag());
    Flags flags;
    if (metaEnum.is64Bit()) {
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            std::optional<quint64> value = metaEnum.value64(i);
            Q_ASSERT(value.has_value());
            flags |= static_cast<Enum>(value.value());
        }
    } else {
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            int value = metaEnum.value(i);
            flags |= static_cast<Enum>(value);
        }
    }
    return flags;
}

[[nodiscard]] HKHeadsetError hidError(HKHeadset::Capability capability, QAnyStringView details)
{
    return {.headset = nullptr,
            .headsetId = 0,
            .headsetName = {},
            .capability = capability,
            .error = HKHeadsetError::HidError,
            .errorString = u"HID communication error"_s,
            .details = details.toString(),
            .timestamp = {}};
}

[[nodiscard]] HKHeadsetError deviceOfflineError(HKHeadset::Capability capability, QAnyStringView details)
{
    return {.headset = nullptr,
            .headsetId = 0,
            .headsetName = {},
            .capability = capability,
            .error = HKHeadsetError::DeviceOffline,
            .errorString = u"Device is offline or not responding"_s,
            .details = details.toString(),
            .timestamp = {}};
}

[[nodiscard]] HKHeadsetError timeoutError(HKHeadset::Capability capability, QAnyStringView details)
{
    return {.headset = nullptr,
            .headsetId = 0,
            .headsetName = {},
            .capability = capability,
            .error = HKHeadsetError::Timeout,
            .errorString = u"Operation timed out"_s,
            .details = details.toString(),
            .timestamp = {}};
}
}

class TestDeviceTest : public QObject
{
    Q_OBJECT

    inline static HKHeadsetId testHeadsetId{0xF00B, 0xA00C};

private Q_SLOTS:
    static void testDevice_data()
    {
        QTest::addColumn<quint8>("profile");
        QTest::addColumn<HKHeadset::Capabilities>("capabilities");
        QTest::addColumn<BatteryResult>("battery");
        QTest::addColumn<ChatMixResult>("chatMix");

        for (quint8 i = 0; i <= 10; ++i) {
            auto &row = QTest::newRow(qPrintable(u"profile %1"_s.arg(QString::number(i))));
            row << i;

            // Capabilities
            if (i == 10) {
                HKHeadset::Capabilities caps;
                caps |= HKHeadset::SidetoneCapability;
                caps |= HKHeadset::LightsCapability;
                caps |= HKHeadset::BatteryStatusCapability;
                row << caps;
            } else {
                row << allFlags<HKHeadset::Capability>();
            }

            // Battery
            switch (i) {
            case 0:
                row << BatteryResult{HKBattery{.level = 42,
                                               .status = HKBattery::BatteryAvailable,
                                               .microphoneStatus = HKBattery::MicrophoneUnknown,
                                               .optionalVoltage = 3650,
                                               .optionalTimeToEmpty = (42 * 720) / 100,
                                               .optionalTimeToFull = {}}};
                break;
            case 1:
                row << BatteryResult{hidError(HKHeadset::BatteryStatusCapability, u"Test error condition"_s)};
                break;
            case 2:
                row << BatteryResult{HKBattery{.level = 50,
                                               .status = HKBattery::BatteryCharging,
                                               .microphoneStatus = HKBattery::MicrophoneUnknown,
                                               .optionalVoltage = 3800,
                                               .optionalTimeToEmpty = {},
                                               .optionalTimeToFull = ((100 - 50) * 120) / 100}};
                break;
            case 3:
                row << BatteryResult{
                    HKBattery{.level = 64, .status = HKBattery::BatteryAvailable, .optionalVoltage = {}, .optionalTimeToEmpty = {}, .optionalTimeToFull = {}}};
                break;
            case 4:
                row << BatteryResult{deviceOfflineError(HKHeadset::BatteryStatusCapability, u"Test unavailable"_s)};
                break;
            case 5:
                row << BatteryResult{timeoutError(HKHeadset::BatteryStatusCapability, u"Test timeout"_s)};
                break;
            case 6:
                row << BatteryResult{HKBattery{.level = 100,
                                               .status = HKBattery::BatteryAvailable,
                                               .microphoneStatus = HKBattery::MicrophoneUnknown,
                                               .optionalVoltage = 4200,
                                               .optionalTimeToEmpty = 720,
                                               .optionalTimeToFull = {}}};
                break;
            case 7:
                row << BatteryResult{HKBattery{.level = 10,
                                               .status = HKBattery::BatteryAvailable,
                                               .microphoneStatus = HKBattery::MicrophoneUnknown,
                                               .optionalVoltage = 3400,
                                               .optionalTimeToEmpty = 72,
                                               .optionalTimeToFull = {}}};
                break;
            default:
                row << BatteryResult{
                    HKBattery{.level = 42, .status = HKBattery::BatteryAvailable, .optionalVoltage = {}, .optionalTimeToEmpty = {}, .optionalTimeToFull = {}}};
                break;
            }

            // ChatMix
            if (i == 1) {
                row << ChatMixResult{hidError(HKHeadset::ChatMixStatusCapability, u"Test error condition"_s)};
            } else {
                row << ChatMixResult{HKChatMix{.level = 64, .gameVolumePercent = 50, .chatVolumePercent = 50}};
            }
        }
    }

    static void testDevice()
    {
        QFETCH(quint8, profile);

        HKHeadsetControl::setTestDeviceEnabled(true);
        HKHeadsetControl::setTestProfile(profile);

        QPointer<HKHeadset> headset;
        {
            HKHeadsetControl::refresh();

            QTest::qWait(std::chrono::milliseconds{100});

            const auto headsets = HKHeadsetControl::headsets();

            auto iter = std::ranges::find_if(headsets, [](HKHeadset *headset) {
                return headset->id() == testHeadsetId;
            });

            QCOMPARE_NE(iter, headsets.constEnd());
            headset = *iter;
        }

        QCOMPARE(headset->name(), "HeadsetControl Test device"_L1);
        {
            QFETCH(HKHeadset::Capabilities, capabilities);
            QCOMPARE(headset->capabilities(), capabilities);
        }

        auto errors = headset->errors();

        COMPARE_RESULT(BatteryResult, battery, HKHeadset::BatteryStatusCapability, HKBattery)

        if (profile != 10) {
            COMPARE_RESULT(ChatMixResult, chatMix, HKHeadset::ChatMixStatusCapability, HKChatMix)
        }
    }
};

QTEST_MAIN(TestDeviceTest)
#include "test-device.moc"
