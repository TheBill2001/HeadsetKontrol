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

    ColumnLayout {
        spacing: 20

        HeadsetKontrol.GroupBox {
            id: generalInfoBox
            title: i18n("General information")

            Layout.fillWidth: true

            contentItem: RowLayout {
                Kirigami.FormLayout {
                    Layout.preferredWidth: parent.width / 2

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Device name") + ":"
                        text: "A Very very very long name!"
                    }
                    Controls.Label {
                        Kirigami.FormData.label: i18n("Chat-mix level") + ":"
                        text: "Blah"
                    }
                    Controls.Label {
                        Kirigami.FormData.label: i18n("Battery") + ":"
                        text: "Blah"
                    }
                }

                Kirigami.Separator {
                    Layout.fillHeight:true
                }

                Kirigami.FormLayout {
                    Layout.preferredWidth: parent.width / 2

                    Controls.Label {
                        Kirigami.FormData.label: i18n("Capabilities") + ":"
                        text: "Capabilities\nCapabilities\nCapabilities\nCapabilities"
                    }
                }
            }
        }

        HeadsetKontrol.GroupBox {
            id: actionBox
            title: i18n("Actions")

            Layout.fillWidth: true

            contentItem: Kirigami.FormLayout {
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

            Layout.fillWidth: true

            contentItem: RowLayout{
                Kirigami.FormLayout {
                    Layout.preferredWidth: parent.width / 2

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
                    Layout.preferredWidth: parent.width / 2

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
                            if (value == 1)
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
