import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.gitlab.thebill2001.headsetkontrol

Kirigami.ScrollablePage {
    id: devicesPage

    title: i18nc("@title:window", "Devices")

    actions: [
        Kirigami.Action {
            icon.name: HeadsetControl.isRunning ? "media-playback-stop" : "media-playback-start"
            text: HeadsetControl.isRunning ? i18nc(
                                                 "@action:intoolbar stop update headsetcontrol",
                                                 "Stop") : i18nc(
                                                 "@action:intoolbar start update headsetcontrol",
                                                 "Start")
            onTriggered: HeadsetControl.isRunning ? HeadsetControl.stop(
                                                        ) : HeadsetControl.start()
        },
        Kirigami.Action {
            icon.name: "view-refresh"
            text: i18nc("@action:intoolbar", "Refresh")
            onTriggered: HeadsetControl.refresh()
        }
    ]

    header: Controls.ProgressBar {
        id: timerBar

        Kirigami.Theme.colorSet: Kirigami.Theme.Button
        Kirigami.Theme.inherit: false

        from: 0
        to: 1
        value: HeadsetControl.countDownTime / Config.updateRate

        visible: Config.showCountdownProgress
                 && HeadsetControl.countDownTime > 0

        implicitHeight: Kirigami.Units.gridUnit * 0.25

        contentItem: Item {
            implicitWidth: timerBar.width
            implicitHeight: timerBar.height

            Rectangle {
                width: timerBar.visualPosition * parent.width
                height: parent.height
                color: Kirigami.Theme.alternateBackgroundColor
            }
        }
    }

    footer: Kirigami.InlineMessage {
        id: footerMessage

        showCloseButton: true
        position: Kirigami.InlineMessage.Position.Footer
        type: Kirigami.MessageType.Error

        Connections {
            target: HeadsetControl
            function onParsingErrorOccurred(error) {
                footerMessage.showError(error)
            }

            function onProcessErrorOccurred(error) {
                footerMessage.showError(error)
            }
        }

        function showError(error) {
            if (devicesPage.isCurrentPage) {
                text = error
                visible = true
            }
        }
    }

    Controls.Label {
        text: i18nc("@info", "This is empty")
        opacity: 0.5
        anchors.centerIn: parent
        font.pixelSize: Kirigami.Units.gridUnit * 2
        visible: HeadsetControl.devices.length < 1
    }

    ColumnLayout {
        Kirigami.CardsLayout {
            id: layout

            maximumColumns: 5
            maximumColumnWidth: Kirigami.Units.gridUnit * 5
            minimumColumnWidth: Kirigami.Units.gridUnit * 3

            Repeater {
                model: HeadsetControl.devices
                delegate: Kirigami.Card {
                    id: deviceCard

                    required property Device modelData
                    required property int index

                    banner {
                        titleIcon: "headsetkontrol"
                        title: modelData.product ? modelData.product : i18nc(
                                                       "@label:textbox",
                                                       "Device %1", index)
                    }

                    highlighted: !devicesPage.isCurrentPage
                                 && root.selectedDevice == modelData

                    contentItem: ColumnLayout {
                        Kirigami.FormLayout {
                            Layout.fillWidth: true

                            Controls.Label {
                                Kirigami.FormData.label: i18nc(
                                                             "@label:textbox",
                                                             "Device") + ":"
                                text: deviceCard.modelData.device
                                visible: deviceCard.modelData.device
                            }

                            Controls.Label {
                                Kirigami.FormData.label: i18nc(
                                                             "@label:textbox",
                                                             "Vendor") + ":"
                                text: deviceCard.modelData.vendor
                                visible: deviceCard.modelData.vendor
                            }

                            RowLayout {
                                Kirigami.FormData.label: i18nc(
                                                             "@label:textbox",
                                                             "Chat-Mix") + ":"

                                visible: deviceCard.modelData.capabilities & Device.ChatMix

                                Controls.ProgressBar {
                                    from: 0
                                    to: 128
                                    value: deviceCard.modelData.chatMix

                                    Layout.maximumWidth: Kirigami.Units.gridUnit * 7
                                }

                                Controls.Label {
                                    text: deviceCard.modelData.chatMix
                                }
                            }

                            RowLayout {
                                Kirigami.FormData.label: i18nc("@label",
                                                               "Battery") + ":"

                                visible: deviceCard.modelData.capabilities & Device.Battery

                                Controls.ProgressBar {
                                    from: 0
                                    to: 100
                                    value: deviceCard.modelData.battery.level
                                    visible: deviceCard.modelData.battery.level > -1

                                    Layout.maximumWidth: Kirigami.Units.gridUnit * 7
                                }

                                Controls.Label {
                                    text: {
                                        switch (deviceCard.modelData.battery.status) {
                                        case DeviceBattery.Available:
                                            return `${deviceCard.modelData.battery.level}%`
                                        case DeviceBattery.Charging:
                                            return deviceCard.modelData.battery.level
                                                    > 0 ? i18nc(
                                                              "@label:textbox",
                                                              "Charging %1",
                                                              `${deviceCard.modelData.battery.level}%`) : i18n(
                                                              "Charging")
                                        default:
                                            return i18nc("@label:textbox",
                                                         "Unavailable")
                                        }
                                    }
                                }
                            }
                        }
                    }

                    showClickFeedback: true
                    onClicked: root.openDeviceDetailPage(modelData)
                }
            }
        }
    }
}
