import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as KirigamiFormCard

import com.gitlab.thebill2001.headsetkontrol

KirigamiFormCard.FormCardPage {
    id: deviceDetailPage

    property Device device: null

    title: i18nc('@title:page', 'Device details')

    KirigamiFormCard.FormHeader {
        title: i18n("Information")
    }

    KirigamiFormCard.FormGridContainer {
        Layout.fillWidth: true

        infoCards: [
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18n("Product")
                subtitle: deviceDetailPage.device?.product ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18n("Product ID")
                subtitle: deviceDetailPage.device?.productId ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18n("Vendor")
                subtitle: deviceDetailPage.device?.vendor ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18n("Vendor ID")
                subtitle: deviceDetailPage.device?.vendorId ?? ""
            },
            KirigamiFormCard.FormGridContainer.InfoCard {
                title: i18n("Device")
                subtitle: deviceDetailPage.device?.device ?? ""
            }
        ]
    }

    KirigamiFormCard.FormHeader {
        title: i18n("Chat-Mix")
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

                    text: i18n("Chat-Mix level")
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
        title: i18n("Battery")
        visible: batteryFromCard.visible
    }

    KirigamiFormCard.FormCard {
        id: batteryFromCard

        visible: deviceDetailPage.device?.capabilities & Device.Battery ?? false

        KirigamiFormCard.FormTextDelegate {
            text: i18n("Battery status")

            description: {
                if (!deviceDetailPage.device)
                    return ""

                switch (deviceDetailPage.device.battery.status) {
                case DeviceBattery.Available:
                    return i18n("Available")
                case DeviceBattery.Charging:
                    return i18n("Charging")
                default:
                    return i18n("Unavailable")
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

                    text: i18n("Battery level")
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
        title: i18n("Configurations")
        visible: configFormCard.visible
    }

    KirigamiFormCard.FormCard {
        id: configFormCard

        visible: deviceDetailPage.device?.capabilities
                 & (Device.Light | Device.VoicePrompt | Device.Sidetone
                    | Device.InactiveTime | Device.NotificationSound) ?? false

        KirigamiFormCard.FormSwitchDelegate {
            id: lightDelegate

            text: i18n("Lights")
            visible: deviceDetailPage.device?.capabilities & Device.Light
                     ?? false

            checked: deviceDetailPage.device?.light ?? true
            onCheckedChanged: {
                if (deviceDetailPage.device)
                    deviceDetailPage.device.light = checked
            }
        }

        KirigamiFormCard.FormSwitchDelegate {
            id: voicePromptDelegate

            text: i18n("Voice prompts")
            visible: deviceDetailPage.device?.capabilities & Device.VoicePrompt
                     ?? false
        }

        KirigamiFormCard.FormSpinBoxDelegate {
            id: sidetoneDelegate

            label: i18n("Sidetone")
            from: 0
            to: 128
            visible: deviceDetailPage.device?.capabilities & Device.Sidetone
                     ?? false
        }

        KirigamiFormCard.FormSpinBoxDelegate {
            id: inactiveTimeDelegate

            label: i18n("Inactive time")
            from: 0
            to: 90
            visible: deviceDetailPage.device?.capabilities & Device.InactiveTime
                     ?? false
            textFromValue: function (value, locale) {
                return i18np("%1 minute", "%1 minutes", value)
            }
            valueFromText: function (text, locale) {
                const matches = text.match(/\d+/)
                return matches ? parseInt(matches.join('')) : 0
            }
        }

        FormTextFieldWithButtonDelegate {
            id: notificationDelegate

            visible: deviceDetailPage.device?.capabilities & Device.NotificationSound
                     ?? false
            label: i18n("Notification sound")
            placeholderText: i18n("Sound ID (depends on device)")
            buttonText: i18n("Play")
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18n("Equalizer")
        visible: equalizerFormCard.visible
    }

    KirigamiFormCard.FormCard {
        id: equalizerFormCard

        visible: deviceDetailPage.device?.capabilities & (Device.Equalizer | Device.EqualizerPreset)
                 ?? false

        FormTextFieldWithButtonDelegate {
            id: equalizerDelegate

            visible: deviceDetailPage.device?.capabilities & Device.Equalizer
                     ?? false
            label: i18n("Equalizer curve")
            placeholderText: i18n("Values separated by spaces, or commas")
            buttonText: i18n("Apply")
        }

        KirigamiFormCard.FormComboBoxDelegate {
            id: equalizerPresetDelegate

            visible: deviceDetailPage.device?.capabilities & Device.EqualizerPreset
                     ?? false
            text: i18n("Equalizer preset")
            model: ListModel {
                ListElement {
                    display: "Default"
                    value: 0
                }
                ListElement {
                    display: "Preset 1"
                    value: 1
                }
                ListElement {
                    display: "Preset 2"
                    value: 2
                }
                ListElement {
                    display: "Preset 3"
                    value: 3
                }
            }
            textRole: "display"
            valueRole: "value"
            currentIndex: 0
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18n("Microphone")
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

            text: i18n("Mircophone mute light brightness")
            from: 0
            to: 3
            stepSize: 1
        }

        FormSliderDelegate {
            id: microphoneVolumeDelegate

            visible: deviceDetailPage.device?.capabilities & Device.MicrophoneVolume
                     ?? false

            text: i18n("Mircophone volume")
            from: 0
            to: 128
            stepSize: 1
        }
    }
}
