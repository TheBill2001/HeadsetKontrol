import QtCore
import QtQml
import QtQuick.Layouts
import QtQuick.Dialogs as Dialogs
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as KirigamiFormCard

import com.gitlab.thebill2001.headsetkontrol

KirigamiFormCard.FormCardPage {
    id: headsetcontrolSettingsPage

    title: i18nc('@title:tab', 'headsetcontrol')

    Kirigami.InlineMessage {
        id: headerMessage

        showCloseButton: true
        position: Kirigami.InlineMessage.Position.Header

        Layout.fillWidth: true

        Connections {
            target: HeadsetControl

            function onProcessErrorOccurred(error) {
                headerMessage.showError(error);
            }
        }

        function showError(error) {
            text = error;
            visible = true;
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "HeadsetControl path")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormButtonDelegate {
            icon.name: "document-open"
            text: i18nc("@action:button", "Select HeadsetControl executable path")

            onClicked: fileDialog.open()
        }

        KirigamiFormCard.FormTextDelegate {
            text: i18n("Path")
            description: Config.executablePath
            visible: Config.executablePath.length > 0
        }

        KirigamiFormCard.FormTextDelegate {
            text: i18nc("@label:textbox", "Version")
            description: HeadsetControl.version
            visible: HeadsetControl.version.length > 0
        }

        KirigamiFormCard.FormTextDelegate {
            text: i18nc("@label:textbox", "API version")
            description: HeadsetControl.apiVersion
            visible: HeadsetControl.apiVersion.length > 0
        }

        KirigamiFormCard.FormTextDelegate {
            text: i18nc("@label:textbox", "HID API version")
            description: HeadsetControl.hidApiVersion
            visible: HeadsetControl.hidApiVersion.length > 0
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Update rate")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSectionText {
            text: i18nc("@info:usagetip", "From 0 to 3600, 0 means disabled.")
        }

        KirigamiFormCard.FormSpinBoxDelegate {
            label: i18nc("@label:spinbox", "Update rate")
            from: 0
            to: 3600

            value: Config.updateRate / 1000
            onValueChanged: Config.updateRate = value * 1000

            textFromValue: function (value, locale) {
                return i18ncp("@item:valuesuffix", "%1 second", "%1 seconds", value);
            }
            valueFromText: function (text, locale) {
                const matches = text.match(/\d+/);
                return matches ? parseInt(matches.join('')) : 0;
            }
        }
    }

    Dialogs.FileDialog {
        id: fileDialog
        fileMode: Dialogs.FileDialog.OpenFile
        currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]

        onAccepted: Config.setExecutablePathUrl(selectedFile)
    }
}
