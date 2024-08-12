import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

Controls.GroupBox {
    id: control

    property alias actions: _actions.children

    padding: 10
    topPadding: padding * 2 + labelSeparator.y

    Layout.fillWidth: true

    background: Rectangle {
        id: background

        width: control.width
        height: control.height

        color: Kirigami.Theme.alternateBackgroundColor
        border.color: Kirigami.ColorUtils.linearInterpolation(
                          Kirigami.Theme.backgroundColor,
                          Kirigami.Theme.textColor, 0.15)
        radius: 5
    }

    label: ColumnLayout {
        x: control.leftPadding
        y: control.leftPadding
        width: control.availableWidth

        RowLayout {
            id: _actions
            Kirigami.Heading {
                id: label
                Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
                level: 2
                text: title
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Kirigami.Separator {
            id: labelSeparator
            Layout.fillWidth: true
        }
    }
}
