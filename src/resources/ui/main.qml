import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

import "HeadsetKontrol" as HeadsetKontrol

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Control your headset")

    minimumWidth: Kirigami.Units.gridUnit * 30
    minimumHeight: Kirigami.Units.gridUnit * 20
    maximumWidth: Kirigami.Units.gridUnit * 50
    maximumHeight: Kirigami.Units.gridUnit * 40
    width: Kirigami.Units.gridUnit * 40
    height: Kirigami.Units.gridUnit * 30

    pageStack.initialPage: Kirigami.ScrollablePage {
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
                        Layout.fillWidth: true

                        Controls.Label {
                            Kirigami.FormData.label: i18n("Device name") + ":"
                            text: "A Very very very very very very very very long name!"
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
                        Layout.fillWidth: true

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

                contentItem: {}
            }
        }
    }
}

