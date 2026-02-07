// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick

import org.kde.ki18n
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: root

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Information")
    }

    FormCard.FormCard {
        FormCard.FormTextDelegate {
            text: HKHeadsetControl.version
            description: KI18n.i18nc("@label:textbox", "HeadsetControl version")
        }

        FormCard.FormButtonDelegate {
            text: KI18n.i18ncp("@action:button", "Found %1 supported device", "Found %1 supported devices", HKHeadsetControl.supportedDevices.length)
            description: supportedDevicesOverlaySheet.title

            onClicked: supportedDevicesOverlaySheet.open()

            Kirigami.OverlaySheet {
                id: supportedDevicesOverlaySheet

                title: KI18n.i18nc("@title:window", "Supported devices")
                parent: root

                ListView {
                    model: HKHeadsetControl.supportedDevices
                    implicitWidth: Kirigami.Units.gridUnit * 25

                    spacing: Kirigami.Units.smallSpacing

                    delegate: Kirigami.SelectableLabel {
                        required property int index
                        required property string modelData

                        leftPadding: Kirigami.Units.largeSpacing
                        rightPadding: leftPadding
                        topPadding: index === 0 ? leftPadding : 0
                        bottomPadding: index === ListView.view.count - 1 ? leftPadding : 0

                        text: modelData
                    }
                }
            }
        }
    }

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Options")
    }

    FormCard.FormCard {
        FormCard.FormSpinBoxDelegate {
            id: updateRateDelegate

            label: KI18n.i18nc("@label:spinbox", "Update rate")
            statusMessage: value === 0 ? KI18n.i18nc("@info:status when update rate is 0", "Auto-update is disabled.") : ""

            from: 0
            to: 2 ** 31 - 1
            stepSize: 100
            value: HKConfig.updateRate

            textFromValue: value => HKUtils.millisecondsToLocaleString(value)
            valueFromText: (text, locale) => HKUtils.localeStringToMilliseconds(text, locale)

            onValueChanged: Qt.callLater(() => {
                HKConfig.updateRate = value;
                HKConfig.save();
            })
        }

        FormCard.FormSpinBoxDelegate {
            id: timeoutDelegate

            label: KI18n.i18nc("@label:spinbox", "Timeout")
            statusMessage: value === 0 ? KI18n.i18nc("@info:status when timeout is 0", "Operations will not time out.") : ""

            from: 0
            stepSize: 100
            to: 2 ** 31 - 1
            value: HKConfig.timeout

            textFromValue: value => HKUtils.millisecondsToLocaleString(value)
            valueFromText: (text, locale) => HKUtils.localeStringToMilliseconds(text, locale)

            onValueChanged: Qt.callLater(() => {
                HKConfig.timeout = value;
                HKConfig.save();
            })
        }

        FormCard.FormSwitchDelegate {
            id: discoverAllDelegate

            checked: HKConfig.discoverAll
            description: KI18n.i18nc("@info:usagetip", "Reports all matching USB devices even if multiple devices of the same model are connected.")
            text: KI18n.i18nc("@option:check", "Discover all devices")

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.discoverAll = checked;
                HKConfig.save();
            })
        }

        FormCard.FormSwitchDelegate {
            id: stopOnRefreshErrorDelegate

            checked: HKConfig.stopOnRefreshError
            description: KI18n.i18nc("@info:usagetip", "Stops automatic update when an error occurred during a refresh.")
            text: KI18n.i18nc("@option:check", "Stop on refresh error")

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.stopOnRefreshError = checked;
                HKConfig.save();
            })
        }
    }

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Test Device")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: testDeviceEnabledDelegate

            checked: HKConfig.testDeviceEnabled
            description: KI18n.i18nc("@info:usagetip", "Developer option: Report test (fake) device.")
            text: KI18n.i18nc("@option:check", "Report test device")

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.testDeviceEnabled = checked;
                HKConfig.save();
            })
        }

        FormCard.FormSpinBoxDelegate {
            id: testProfileDelegate

            label: KI18n.i18nc("@option:check", "Test device profile")
            enabled: testDeviceEnabledDelegate.checked
            from: 0
            stepSize: 1
            to: 10
            value: HKConfig.testProfile

            onValueChanged: Qt.callLater(() => {
                HKConfig.testProfile = value;
                HKConfig.save();
            })
        }
    }
}
