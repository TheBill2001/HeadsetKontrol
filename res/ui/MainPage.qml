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
                icon.name: "configure"
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

            actions: [
                Controls.Label{
                    visible: AppController.config.showCountdownTimer
                    text: {
                        var pad = (n, z = 2) => ('00' + n).slice(-z);
                        return pad(AppController.remainingTime/3.6e6|0) + ':' + pad((AppController.remainingTime%3.6e6)/6e4 | 0) + ':' + pad((AppController.remainingTime%6e4)/1000|0) + '.' + pad(AppController.remainingTime%1000, 3);
                    }
                },
                Controls.Button {
                    text: AppController.isPaused ? i18n("Resume") : i18n("Pause")
                    icon.name: AppController.isPaused ? "media-playback-start" : "media-playback-pause"
                    onClicked: {
                        AppController.pauseToggle();
                    }
                },
                Controls.Button {
                    text: i18n("Refresh")
                    icon.name: "view-refresh"
                    onClicked: {
                        AppController.headsetControl.queryAll();
                    }
                }
            ]

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
                            if (capStr !== "")
                                return capStr.trim();
                            return i18n("Unavailable")
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

            actions: [
                Controls.Button {
                    text: i18n("Default")
                    icon.name: "kt-restore-defaults"
                    onClicked: {
                        AppController.resetHeadsetSettings();

                        rotateToMuteCheckBox.checked = AppController.config.rotateToMute;
                        voicePromptCheckBox.checked = AppController.config.voicePrompt;
                        ledCheckBox.checked = AppController.config.led;
                        sidetoneSpinBox.value = AppController.config.sidetone;
                        inactiveTimeSpinBox.value = AppController.config.inactiveTime;

                        AppController.headsetControl.setRotateToMute(rotateToMuteCheckBox.checked);
                        AppController.headsetControl.setVoicePrompt(voicePromptCheckBox.checked);
                        AppController.headsetControl.setLed(ledCheckBox.checked);
                        AppController.headsetControl.setSidetone(sidetoneSpinBox.value);
                        AppController.headsetControl.setInactiveTime(inactiveTimeSpinBox.value);
                    }
                },
                Controls.Button {
                    text: i18n("Apply")
                    icon.name: "dialog-ok-apply"
                    onClicked: {
                        AppController.config.rotateToMute = rotateToMuteCheckBox.checked;
                        AppController.config.voicePrompt = voicePromptCheckBox.checked;
                        AppController.config.led = ledCheckBox.checked;
                        AppController.config.sidetone = sidetoneSpinBox.value;
                        AppController.config.inactiveTime = inactiveTimeSpinBox.value;

                        AppController.headsetControl.setRotateToMute(rotateToMuteCheckBox.checked);
                        AppController.headsetControl.setVoicePrompt(voicePromptCheckBox.checked);
                        AppController.headsetControl.setLed(ledCheckBox.checked);
                        AppController.headsetControl.setSidetone(sidetoneSpinBox.value);
                        AppController.headsetControl.setInactiveTime(inactiveTimeSpinBox.value);

                        AppController.saveSettings();
                    }
                }
            ]

            RowLayout{
                Kirigami.FormLayout {
                    Layout.preferredWidth: (settingsBox.availableWidth) / 2

                    Controls.CheckBox {
                        id: rotateToMuteCheckBox
                        text: i18n("Rotate to mute")
                        checked: AppController.config.rotateToMute
                        enabled: AppController.headsetControl.hasRotateToMuteCapabilitiy
                    }

                    Controls.CheckBox {
                        id: voicePromptCheckBox
                        text: i18n("Voice prompt")
                        checked: AppController.config.voicePrompt
                        enabled: AppController.headsetControl.hasVoicePromptCapabilitiy
                    }

                    Controls.CheckBox {
                        id: ledCheckBox
                        text: i18n("LED")
                        checked: AppController.config.led
                        enabled: AppController.headsetControl.hasLedCapability
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
                    }

                    RowLayout {
                        Kirigami.FormData.label: i18n("Inactive time") + ":"

                        Controls.SpinBox {
                            id: inactiveTimeSpinBox
                            from: 0
                            to: 90
                            value: AppController.config.inactiveTime
                            enabled: AppController.headsetControl.hasInactiveTimeCapabilities
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
