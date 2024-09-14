import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as KirigamiFormCard

import com.gitlab.thebill2001.headsetkontrol

KirigamiFormCard.FormCardPage {
    id: deviceDetailPage

    property Device device: null

    title: i18nc("@title:window", "Device details")

    actions: [
        Kirigami.Action {
            id: reApplyAction
        },
        Kirigami.Action {
            id: setPrimaryAction
        }
    ]

    Connections {
        target: deviceDetailPage
        function onDeviceChanged() {
            if (device) {
                reApplyAction.fromQAction = device.reApplyAction
                setPrimaryAction.fromQAction = device.setPrimaryAction
            }
        }
    }

    header: Kirigami.InlineMessage {
        id: headerMessage

        showCloseButton: true
        position: Kirigami.InlineMessage.Position.Header
        type: Kirigami.MessageType.Error

        Connections {
            target: HeadsetControl
            function onParsingErrorOccurred(error) {
                headerMessage.showError(error)
            }

            function onProcessErrorOccurred(error) {
                headerMessage.showError(error)
            }
        }

        function showError(error) {
            if (deviceDetailPage.isCurrentPage) {
                text = error
                visible = true
            }
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Information")
    }

    KirigamiFormCard.FormGridContainer {
        Layout.fillWidth: true

        infoCards: [
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18nc("@label:textbox", "Product")
                subtitle: deviceDetailPage.device?.product ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18nc("@label:textbox", "Product ID")
                subtitle: deviceDetailPage.device?.productId ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18nc("@label:textbox", "Vendor")
                subtitle: deviceDetailPage.device?.vendor ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18nc("@label:textbox", "Vendor ID")
                subtitle: deviceDetailPage.device?.vendorId ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18nc("@label:textbox", "Device")
                subtitle: deviceDetailPage.device?.device ?? ""
            }
        ]
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Chat-Mix")
        visible: chatMixFormCard.visible
    }

    KirigamiFormCard.FormCard {
        id: chatMixFormCard

        visible: deviceDetailPage.device?.capabilities & Device.ChatMix ?? false

        KirigamiFormCard.AbstractFormDelegate {
            visible: deviceDetailPage.device?.battery.level > -1 ?? false
            hoverEnabled: false
            background: null

            contentItem: ColumnLayout {
                spacing: Kirigami.Units.smallSpacing

                Controls.Label {
                    Layout.fillWidth: true

                    text: i18nc("@label", "Chat-Mix level")
                }

                RowLayout {
                    Controls.ProgressBar {
                        Layout.fillWidth: true

                        from: 0
                        to: 128
                        value: deviceDetailPage.device?.chatMix ?? 0
                    }

                    Controls.Label {
                        Layout.leftMargin: Kirigami.Units.smallSpacing

                        text: deviceDetailPage.device?.chatMix ?? ""
                    }
                }
            }
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Battery")
        visible: batteryFromCard.visible
    }

    KirigamiFormCard.FormCard {
        id: batteryFromCard

        visible: deviceDetailPage.device?.capabilities & Device.Battery ?? false

        KirigamiFormCard.FormTextDelegate {
            text: i18nc("@label", "Battery status")

            description: {
                if (!deviceDetailPage.device)
                    return ""

                switch (deviceDetailPage.device.battery.status) {
                case DeviceBattery.Available:
                    return i18nc("@item battery status", "Available")
                case DeviceBattery.Charging:
                    return i18nc("@item battery status", "Charging")
                default:
                    return i18nc("@item battery status", "Unavailable")
                }
            }
        }

        KirigamiFormCard.AbstractFormDelegate {
            visible: deviceDetailPage.device?.battery.level > -1 ?? false
            hoverEnabled: false
            background: null

            contentItem: ColumnLayout {
                spacing: Kirigami.Units.smallSpacing

                Controls.Label {
                    Layout.fillWidth: true

                    text: i18nc("@label", "Battery level")
                }

                RowLayout {
                    Controls.ProgressBar {
                        Layout.fillWidth: true

                        from: 0
                        to: 100
                        value: deviceDetailPage.device?.battery.level ?? 0
                    }

                    Controls.Label {
                        Layout.leftMargin: Kirigami.Units.smallSpacing

                        text: `${deviceDetailPage.device?.battery.level ?? 0}%`
                    }
                }
            }
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Configurations")
        visible: configFormCard.visible
    }

    KirigamiFormCard.FormCard {
        id: configFormCard

        visible: deviceDetailPage.device?.capabilities
                 & (Device.Light | Device.VoicePrompt | Device.Sidetone
                    | Device.InactiveTime | Device.NotificationSound) ?? false

        KirigamiFormCard.FormSwitchDelegate {
            id: lightDelegate

            text: i18nc("@option:check", "Lights")
            visible: deviceDetailPage.device?.capabilities & Device.Light
                     ?? false

            checked: deviceDetailPage.device?.light ?? true
            onCheckedChanged: {
                if (deviceDetailPage.device)
                    deviceDetailPage.device.light = lightDelegate.checked
            }
        }

        KirigamiFormCard.FormSwitchDelegate {
            id: voicePromptDelegate

            text: i18nc("@option:check", "Voice prompts")
            visible: deviceDetailPage.device?.capabilities & Device.VoicePrompt
                     ?? false
            checked: deviceDetailPage.device?.voicePrompt ?? true
            onCheckedChanged: {
                if (deviceDetailPage.device)
                    deviceDetailPage.device.voicePrompt = voicePromptDelegate.checked
            }
        }

        KirigamiFormCard.FormSpinBoxDelegate {
            id: sidetoneDelegate

            label: i18nc("@label:spinbox", "Sidetone")
            from: 0
            to: 128
            visible: deviceDetailPage.device?.capabilities & Device.Sidetone
                     ?? false

            value: deviceDetailPage.device?.sidetone ?? 0
            onValueChanged: {
                if (deviceDetailPage.device)
                    deviceDetailPage.device.sidetone = sidetoneDelegate.value
            }
        }

        KirigamiFormCard.FormSpinBoxDelegate {
            id: inactiveTimeDelegate

            label: i18nc("@label:spinbox", "Inactive time")
            from: 0
            to: 90
            visible: deviceDetailPage.device?.capabilities & Device.InactiveTime
                     ?? false
            textFromValue: function (value, locale) {
                return i18ncp("@item:valuesuffix", "%1 minute",
                              "%1 minutes", value)
            }
            valueFromText: function (text, locale) {
                const matches = text.match(/\d+/)
                return matches ? parseInt(matches.join('')) : 0
            }

            value: deviceDetailPage.device?.inactiveTime ?? 0
            onValueChanged: {
                if (deviceDetailPage.device)
                    deviceDetailPage.device.inactiveTime = inactiveTimeDelegate.value
            }
        }

        FormTextFieldWithButtonDelegate {
            id: notificationDelegate

            visible: deviceDetailPage.device?.capabilities & Device.NotificationSound
                     ?? false
            label: i18nc("@label:textbox", "Notification sound")
            placeholderText: i18nc("@info:placeholder",
                                   "Sound ID (depends on device)")
            buttonText: i18nc("@action:button", "Play")

            onAccepted: {
                if (deviceDetailPage.device) {
                    if (!notificationDelegate.text) {
                        headerMessage.showError(
                                    i18nc("@info", "Sound ID cannot be empty."))
                    } else {
                        deviceDetailPage.device.playNotification(
                                    notificationDelegate.text)
                    }
                }
            }
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Equalizer")
        visible: equalizerFormCard.visible
    }

    KirigamiFormCard.FormCard {
        id: equalizerFormCard

        visible: deviceDetailPage.device?.capabilities & (Device.Equalizer | Device.EqualizerPreset)
                 ?? false

        // No idea what the validator is
        FormTextFieldWithButtonDelegate {
            id: equalizerDelegate

            visible: deviceDetailPage.device?.capabilities & Device.Equalizer
                     ?? false
            label: i18nc("@label:textbox", "Equalizer curve")
            placeholderText: i18nc("@info:placeholder",
                                   "Values separated by spaces, or commas")
            buttonText: i18nc("@action:button", "Apply")

            text: deviceDetailPage.device?.equalizer ?? ""
            onAccepted: {
                if (deviceDetailPage.device) {
                    if (!equalizerDelegate.text) {
                        headerMessage.showError(
                                    i18nc("@info",
                                          "Equalizer curve cannot be empty."))
                    } else {
                        deviceDetailPage.device.equalizer = equalizerDelegate.text
                    }
                }
            }
        }

        KirigamiFormCard.FormComboBoxDelegate {
            id: equalizerPresetDelegate

            visible: deviceDetailPage.device?.capabilities & Device.EqualizerPreset
                     ?? false
            text: i18nc("@label:listbox", "Equalizer preset")
            model: [{
                    "display": i18nc("@item:inlistbox", "Default"),
                    "value": 0
                }, {
                    "display": i18nc("@item:inlistbox", "Preset %1", 1),
                    "value": 1
                }, {
                    "display": i18nc("@item:inlistbox", "Preset %1", 2),
                    "value": 2
                }, {
                    "display": i18nc("@item:inlistbox", "Preset %1", 3),
                    "value": 3
                }]
            textRole: "display"
            valueRole: "value"
            currentIndex: deviceDetailPage.device?.equalizerPreset ?? 0
            onActivated: function (index) {
                if (deviceDetailPage.device) {
                    deviceDetailPage.device.equalizerPreset = equalizerPresetDelegate.currentIndex
                }
            }
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Microphone")
        visible: mircophoneFormCard.visible
    }

    KirigamiFormCard.FormCard {
        id: mircophoneFormCard

        visible: deviceDetailPage.device?.capabilities
                 & (Device.MicrophoneMuteLedBrightness | Device.MicrophoneVolume)
                 ?? false

        FormSliderDelegate {
            id: microphoneLightBrightnessDelegate

            visible: deviceDetailPage.device?.capabilities & Device.MicrophoneMuteLedBrightness
                     ?? false

            text: i18nc("@label:slider", "Mircophone mute light brightness")
            from: 0
            to: 3
            stepSize: 1
            value: deviceDetailPage.device?.microphoneBrightness ?? 3
            onMoved: {
                if (deviceDetailPage.device) {
                    deviceDetailPage.device.microphoneBrightness
                            = microphoneLightBrightnessDelegate.value
                }
            }
        }

        FormSliderDelegate {
            id: microphoneVolumeDelegate

            visible: deviceDetailPage.device?.capabilities & Device.MicrophoneVolume
                     ?? false

            text: i18nc("@label:slider", "Mircophone volume")
            from: 0
            to: 128
            stepSize: 1
            value: deviceDetailPage.device?.microphoneVolume ?? 3
            onMoved: {
                if (deviceDetailPage.device) {
                    deviceDetailPage.device.microphoneVolume = microphoneVolumeDelegate.value
                }
            }
        }
    }
}
