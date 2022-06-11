import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

import "HeadsetKontrol" as HeadsetKontrol

Kirigami.ScrollablePage {
    id: mainPage
    actions {
        contextualActions: [
            Kirigami.Action {
                id: settingsAction
                icon.name: "settings"
                text: i18nc("@action:button", "Settings")
                onTriggered: root.pageStack.pushDialogLayer("qrc:/resources/ui/SettingsPage/SettingsPage.qml")
            },
            Kirigami.Action {
                id: quitAction
                icon.name: "gtk-quit"
                text: i18nc("@action:button", "Quit")
                onTriggered: Qt.callLater(Qt.quit())
            }
        ]
    }

    titleDelegate: Kirigami.Heading {
        text: "Headset controls"
        leftPadding: 5
    }

    Timer {
        id: timer
        interval: Config.updateRate
        running: true
        repeat: true
        onTriggered: {
            Instance.queryAll()
        }
    }

    ColumnLayout {
        spacing: 20

        HeadsetKontrol.GroupBox {
            id: generalInfoBox
            title: i18n("General information")

            RowLayout {
                Kirigami.FormLayout {
                    Layout.preferredWidth: (generalInfoBox.availableWidth) / 2

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Device name") + ":"
                        text: Instance.name !== "" ? Instance.name : i18n("Unavailable")
                    }

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Chat-mix level") + ":"
                        text: Instance.chatMix > -1 ? Instance.chatMix : i18n("Unavailable")
                    }

                    RowLayout {
                        Kirigami.FormData.label: i18n("Battery") + ":"
                        Controls.ProgressBar {
                            implicitWidth: Kirigami.Units.gridUnit * 5
                            from: 0
                            to: 100
                            value: Instance.battery
                            indeterminate: Instance.battery < 0
                        }
                        Controls.Label {
                            text: {
                                if (Instance.battery === -1)
                                    return i18n("Charging");
                                if (Instance.battery === -2)
                                    return i18n("Unavailable");
                                return Instance.battery + "%"
                            }
                        }
                    }
                }

                Kirigami.Separator {
                    Layout.fillHeight: true
                }

                Kirigami.FormLayout {
                    Layout.preferredWidth: (generalInfoBox.availableWidth) / 2

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Capabilities") + ":"
                        text: {
                            var capStr = "";
                            if (Instance.hasSidetoneCapability)
                                capStr += i18n("Sidetone") + "\n";
                            if (Instance.hasBatteryCapability)
                                capStr += i18n("Battery") + "\n";
                            if (Instance.hasNotificationSoundCapability)
                                capStr += i18n("Play notification sound") + "\n";
                            if (Instance.hasLedCapability)
                                capStr += i18n("Set LED mode") + "\n";
                            if (Instance.hasInactiveTimeCapabilities)
                                capStr += i18n("Set inactive time") + "\n";
                            if (Instance.hasChatMixCapabilitiy)
                                capStr += i18n("Chat-Mix") + "\n";
                            if (Instance.hasVoicePromptCapabilitiy)
                                capStr += i18n("Set voice prompt mode") + "\n";
                            if (Instance.hasRotateToMuteCapabilitiy)
                                capStr += i18n("Rotate-To-Mute") + "\n";
                            if (Instance.hasEqualizerPresetCapability)
                                capStr += i18n("Set equalizer preset") + "\n";
                            return capStr.trim();
                        }
                    }
                }
            }
        }

        HeadsetKontrol.GroupBox {
            id: actionBox
            title: i18n("Actions")

            contentItem: Kirigami.FormLayout { // If use a layout, remove it from contentItem
                RowLayout {
                    Kirigami.FormData.label: i18n("Notification sound:")
                    Controls.ComboBox {
                        model: ["Sound 1", "Sound 2"]
                    }
                    Controls.Button {
                        text: i18n("Play")
                    }
                }
            }
        }

        HeadsetKontrol.GroupBox {
            id: settingsBox
            title: i18n("Settings")

            RowLayout{
                Kirigami.FormLayout {
                    Layout.preferredWidth: (settingsBox.availableWidth) / 2

                    Controls.CheckBox {
                        id: rotateToMuteCheckBox
                        text: i18n("Rotate to mute")
                        checked: Config.rotateToMute

                        onClicked: {
                            Config.rotateToMute = rotateToMuteCheckBox.checked;
                            Config.save();
                        }
                    }

                    Controls.CheckBox {
                        id: voicePromptCheckBox
                        text: i18n("Voice prompt")
                        checked: Config.voicePrompt

                        onClicked: {
                            Config.voicePrompt = voicePromptCheckBox.checked;
                            Config.save();
                        }
                    }

                    Controls.CheckBox {
                        id: ledCheckBox
                        text: i18n("LED")
                        checked: Config.led

                        onClicked: {
                            Config.led = ledCheckBox.checked;
                            Config.save();
                        }
                    }
                }

                Kirigami.Separator {
                    Layout.fillHeight:true
                }

                Kirigami.FormLayout {
                    Layout.preferredWidth: (settingsBox.availableWidth) / 2

                    Controls.SpinBox {
                        id: sidetoneSpinBox
                        Kirigami.FormData.label: i18n("Sidetone level") + ":"

                        from: 0
                        to: 128
                        value: Config.sidetone

                        onValueModified: {
                            Config.sidetone = sidetoneSpinBox.value;
                            Config.save();
                        }
                    }

                    Controls.SpinBox {
                        id: inactiveTimeSpinBox
                        Kirigami.FormData.label: i18n("Inactive time") + ":"
                        from: 0
                        to: 90
                        value: Config.inactiveTime
                        textFromValue: function(value, locale) {
                            if (value === 1)
                                return Number(value).toLocaleString(locale, 'f', 0) + " " +i18n("minute");
                            return Number(value).toLocaleString(locale, 'f', 0) + " " + i18n("minutes");
                        }

                        onValueModified: {
                            Config.inactiveTime = inactiveTimeSpinBox.value;
                            Config.save();
                        }
                    }
                }
            }
        }
    }
}
