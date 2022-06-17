import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

Kirigami.ScrollablePage {
    id: generalSettingsPage
    title: i18nc('@title:window', 'General')
    Kirigami.FormLayout {
        Layout.fillWidth: true

        Item {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "HeadsetControl options"
        }


        Kirigami.ActionTextField {
            id: binPathTextField

            Kirigami.FormData.label: i18n("HeadsetControl path") + ":"
            placeholderText: i18n("Path to HeadsetControl") + "..."
            text: AppController.config.binPath

            rightActions: [
                Kirigami.Action {
                    icon.name: "document-open"

                    onTriggered: fileDialog.open()
                }
            ]

            onAccepted: {
                AppController.config.binPath = binPathTextField.text
            }
        }

        RowLayout {
            Kirigami.FormData.label: i18n("Update rate (seconds)") + ":"

            Controls.SpinBox {
                id: updateRateSpinBox

                from: 0
                to: 3600
                value: AppController.config.updateRate / 1000

                onValueModified: {
                    AppController.config.updateRate = updateRateSpinBox.value * 1000
                }
            }

            Controls.Label {
                text: i18n("From 0 to 3600, 0 means disable")
                font.italic: true
            }
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "General options"
        }

        Controls.CheckBox {
            Kirigami.FormData.label: i18n("Run in backgroud") + ":"
            checked: AppController.config.runInBackground

            onToggled: {
                AppController.config.runInBackground = checked;
            }
        }

        Controls.CheckBox {
            Kirigami.FormData.label: i18n("Autostart") + ":"
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: "UI options"
        }

        Controls.CheckBox {
            Kirigami.FormData.label: i18n("Show countdown timer") + ":"
            checked: AppController.config.showCountdownTimer

            onToggled: {
                AppController.config.showCountdownTimer = checked;
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: i18nc('@title:dialog', 'Select path to HeadsetControl')
        selectMultiple: false

        onAccepted: {
            binPathTextField.text = fileDialog.fileUrl
            binPathTextField.accepted()
        }
    }
}
