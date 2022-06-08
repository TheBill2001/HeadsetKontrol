import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

Rectangle {
    id: background

    property string title
    property alias contentItem: loader.sourceComponent
    property int paddings: 10

    implicitWidth: layout.implicitWidth + paddings * 2
    implicitHeight: layout.implicitHeight + paddings * 2

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    color: Kirigami.Theme.alternateBackgroundColor
    border.color: Kirigami.ColorUtils.linearInterpolation(Kirigami.Theme.backgroundColor, Kirigami.Theme.textColor, 0.07)
    radius: 5

    ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.margins: paddings

        Kirigami.Heading {
            text: background.title
            level: 2
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }

        Loader {
            id: loader
            Layout.fillWidth: true
        }
    }
}


//Controls.GroupBox {
    //id: control

    //Kirigami.Theme.colorSet: Kirigami.Theme.View
    //Kirigami.Theme.inherit: false

    //background: Rectangle {
        //y: control.topPadding - control.bottomPadding
        //width: parent.width
        //height: parent.height - control.topPadding + control.bottomPadding

        //color: Kirigami.Theme.alternateBackgroundColor

        //radius: 5
    //}

    //label: Kirigami.Heading {
        //text: control.title
        //x: control.leftPadding
        //width: control.availableWidth

        //level: 2
    //}
//}