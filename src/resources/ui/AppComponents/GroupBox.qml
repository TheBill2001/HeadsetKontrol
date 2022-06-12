import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

Controls.GroupBox {
    id: control

    padding: 10
    topPadding: padding * 2 + labelSeparator.y

    Layout.fillWidth: true

    background: Rectangle {
        id: background

        width: control.width
        height: control.height

        color: Kirigami.Theme.alternateBackgroundColor
        border.color: Kirigami.ColorUtils.linearInterpolation(Kirigami.Theme.backgroundColor, Kirigami.Theme.textColor, 0.15)
        radius: 5
    }

    label: Kirigami.Heading {
        id: label

        x: control.leftPadding
        y: control.leftPadding

        width: control.availableWidth

        level: 2
        text: title

        Kirigami.Separator {
            id: labelSeparator

            y: label.height + control.padding / 2
            width: label.width
        }
    }
}


//ColumnLayout {
//    id: layout

//    property string title
//    property real padding: 10

//    Rectangle {
//        id: background

//        width: layout.width
//        height: layout.height

//        color: Kirigami.Theme.alternateBackgroundColor
//        border.color: Kirigami.ColorUtils.linearInterpolation(Kirigami.Theme.backgroundColor, Kirigami.Theme.textColor, 0.15)
//        radius: 5
//    }

//    Kirigami.Heading {
//        id: label

//        Layout.leftMargin: layout.padding
//        Layout.rightMargin: layout.padding
//        Layout.topMargin: layout.padding
//        Layout.fillWidth: true

//        level: 2
//        text: title
//    }

//    Kirigami.Separator {
//        Layout.fillWidth: true
//        Layout.leftMargin: layout.padding
//        Layout.rightMargin: layout.padding
//        Layout.bottomMargin: layout.padding
//    }
//}
