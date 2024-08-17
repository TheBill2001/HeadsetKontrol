import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as KirigamiFormCard

RowLayout {
    id: delegate

    property alias label: textField.label
    property alias placeholderText: textField.placeholderText
    property alias buttonText: button.text
    property alias text: textField.text
    property alias validator: textField.validator

    signal accepted

    // Good grief with the spacing
    KirigamiFormCard.FormTextFieldDelegate {
        id: textField

        Layout.rightMargin: Kirigami.Units.smallSpacing * -4

        onAccepted: delegate.accepted()
        onEditingFinished: delegate.accepted()
    }

    Controls.Button {
        id: button

        Layout.rightMargin: Kirigami.Units.smallSpacing * 4
        Layout.bottomMargin: Kirigami.Units.smallSpacing * 3
        Layout.alignment: Qt.AlignBottom

        onClicked: delegate.accepted()
    }
}
