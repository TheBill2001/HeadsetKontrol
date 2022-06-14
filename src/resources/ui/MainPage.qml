import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

import "AppComponents" as AppComponents

Kirigami.ScrollablePage {
    id: mainPage
    actions {
        contextualActions: [
            Kirigami.Action {
                id: settingsAction
                icon.name: "settings"
                text: i18nc("@action:button", "Settings")
                onTriggered: root.pageStack.pushDialogLayer(settingsPageComponent, {}, {
                                                                "minimumWidth": Kirigami.Units.gridUnit * 30,
                                                                "minimumHeight": Kirigami.Units.gridUnit * 20,
                                                                "maximumWidth": Kirigami.Units.gridUnit * 50,
                                                                "maximumHeight": Kirigami.Units.gridUnit * 40,
                                                                "width": Kirigami.Units.gridUnit * 40,
                                                                "height": Kirigami.Units.gridUnit * 30
                                                            })
            },
            Kirigami.Action {
                id: quitAction
                icon.name: "gtk-quit"
                text: i18nc("@action:button", "Quit")
                onTriggered: Qt.quit()
            }
        ]
    }

    titleDelegate: Kirigami.Heading {
        text: i18n("Headset controls")
        leftPadding: 5
    }

    ColumnLayout {
        spacing: 20

        AppComponents.GroupBox {
            id: generalInfoBox
            title: i18n("General information")

            RowLayout {
                Kirigami.FormLayout {
                    Layout.preferredWidth: generalInfoBox.availableWidth / 2

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Device name") + ":"
                        text: AppController.headsetControl.name !== "" ? AppController.headsetControl.name : i18n("Unavailable")
                    }

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Chat-mix level") + ":"
                        text: AppController.headsetControl.chatMix > -1 ? AppController.headsetControl.chatMix : i18n("Unavailable")
                    }

                    RowLayout {
                        Kirigami.FormData.label: i18n("Battery") + ":"
                        Controls.ProgressBar {
                            implicitWidth: Kirigami.Units.gridUnit * 5
                            from: 0
                            to: 100
                            value: AppController.headsetControl.battery
                            indeterminate: AppController.headsetControl.battery < 0
                        }
                        Controls.Label {
                            text: {
                                if (AppController.headsetControl.battery === -1)
                                    return i18n("Charging");
                                if (AppController.headsetControl.battery === -2)
                                    return i18n("Unavailable");
                                return AppController.headsetControl.battery + "%"
                            }
                        }
                    }
                }

                Kirigami.Separator {
                    Layout.fillHeight: true
                }

                Kirigami.FormLayout {
                    Layout.preferredWidth: generalInfoBox.availableWidth / 2

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Capabilities") + ":"
                        text: {
                            var capStr = "";
                            if (AppController.headsetControl.hasSidetoneCapability)
                                capStr += i18n("Sidetone") + "\n";
                            if (AppController.headsetControl.hasBatteryCapability)
                                capStr += i18n("Battery") + "\n";
                            if (AppController.headsetControl.hasNotificationSoundCapability)
                                capStr += i18n("Play notification sound") + "\n";
                            if (AppController.headsetControl.hasLedCapability)
                                capStr += i18n("Set LED mode") + "\n";
                            if (AppController.headsetControl.hasInactiveTimeCapabilities)
                                capStr += i18n("Set inactive time") + "\n";
                            if (AppController.headsetControl.hasChatMixCapabilitiy)
                                capStr += i18n("Chat-Mix") + "\n";
                            if (AppController.headsetControl.hasVoicePromptCapabilitiy)
                                capStr += i18n("Set voice prompt mode") + "\n";
                            if (AppController.headsetControl.hasRotateToMuteCapabilitiy)
                                capStr += i18n("Rotate-To-Mute") + "\n";
                            if (AppController.headsetControl.hasEqualizerPresetCapability)
                                capStr += i18n("Set equalizer preset") + "\n";
                            return capStr.trim();
                        }
                    }
                }
            }
        }

        AppComponents.GroupBox {
            id: actionBox
            title: i18n("Actions")

            contentItem: Kirigami.FormLayout {
                enabled: AppController.headsetControl.hasNotificationSoundCapability

                Controls.ComboBox {
                    Kirigami.FormData.label: i18n("Notification sound") + ":"
                    model: ["Sound 1", "Sound 2"]

                    Controls.Button {   // workaround weird polish loop
                        x: parent.width + 5
                        height: parent.height
                        text: i18n("Play")
                        onClicked: {
                            AppController.headsetControl.setNotificationSound(parent.currentIndex);
                        }
                    }
                }
            }
        }

        AppComponents.GroupBox {
            id: settingsBox
            title: i18n("Settings")

            RowLayout{
                Kirigami.FormLayout {
                    Layout.preferredWidth: (settingsBox.availableWidth) / 2

                    Controls.CheckBox {
                        id: rotateToMuteCheckBox
                        text: i18n("Rotate to mute")
                        checked: AppController.config.rotateToMute
                        enabled: AppController.headsetControl.hasRotateToMuteCapabilitiy

                        onClicked: {
                            AppController.config.rotateToMute = rotateToMuteCheckBox.checked;
                            AppController.headsetControl.setRotateToMute(rotateToMuteCheckBox.checked);
                            AppController.config.save();
                        }
                    }

                    Controls.CheckBox {
                        id: voicePromptCheckBox
                        text: i18n("Voice prompt")
                        checked: AppController.config.voicePrompt
                        enabled: AppController.headsetControl.hasVoicePromptCapabilitiy

                        onClicked: {
                            AppController.config.voicePrompt = voicePromptCheckBox.checked;
                            AppController.headsetControl.setVoicePromp(voicePromptCheckBox.checked);
                            AppController.config.save();
                        }
                    }

                    Controls.CheckBox {
                        id: ledCheckBox
                        text: i18n("LED")
                        checked: AppController.config.led

                        onClicked: {
                            AppController.config.led = ledCheckBox.checked;
                            AppController.headsetControl.setLed(ledCheckBox.checked);
                            AppController.config.save();
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
                        enabled: AppController.headsetControl.hasSidetoneCapability

                        from: 0
                        to: 128
                        value: AppController.config.sidetone

                        onValueModified: {
                            AppController.config.sidetone = sidetoneSpinBox.value;
                            AppController.headsetControl.setSidetone(sidetoneSpinBox.value);
                            AppController.config.save();
                        }
                    }

                    RowLayout {
                        Kirigami.FormData.label: i18n("Inactive time") + ":"

                        Controls.SpinBox {
                            id: inactiveTimeSpinBox
                            from: 0
                            to: 90
                            value: AppController.config.inactiveTime
                            enabled: AppController.headsetControl.hasInactiveTimeCapabilities

                            onValueModified: {
                                AppController.config.inactiveTime = inactiveTimeSpinBox.value;
                                AppController.headsetControl.setInactiveTime(inactiveTimeSpinBox.value);
                                AppController.config.save();
                            }
                        }

                        Controls.Label {    // Use spin box textFromValue sometime reset the value to 0 after confirm. BUG?
                            text: i18np("minute", "minutes", inactiveTimeSpinBox.value)
                        }
                    }
                }
            }
        }
    }
}
