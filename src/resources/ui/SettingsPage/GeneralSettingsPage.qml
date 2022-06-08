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

        Kirigami.ActionTextField {
            id: binPathTextField

            Kirigami.FormData.label: i18n("HeadsetControl path") + ":"
            placeholderText: i18n("Path to HeadsetControl") + "..."
            text: Config.binPath

            rightActions: [
                Kirigami.Action {
                    icon.name: "document-open"

                    onTriggered: fileDialog.open()
                }
            ]

            onAccepted: {
                Config.binPath = binPathTextField.text
            }
        }

        RowLayout {
            Kirigami.FormData.label: i18n("Update rate (seconds)") + ":"

            Controls.SpinBox {
                id: updateRateSpinBox

                from: 0
                to: 3600
                value: Config.updateRate / 1000

                onValueModified: {
                    Config.updateRate = updateRateSpinBox.value * 1000
                }
            }

            Controls.Label {
                text: i18n("From 0 to 3600, 0 means disable")
                font.italic: true
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
