import QtQuick 2.15
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

import "SettingsPage"

Kirigami.ApplicationWindow {
    id: root

    property string mainPageComponent: "qrc:/resources/qml/MainPage.qml"
    property string settingsPageComponent: "qrc:/resources/qml/SettingsPage/SettingsPage.qml"

    title: i18nc("@title:window", "Control your headset")

    minimumWidth: Kirigami.Units.gridUnit * 30
    minimumHeight: Kirigami.Units.gridUnit * 20
    maximumWidth: Kirigami.Units.gridUnit * 50
    maximumHeight: Kirigami.Units.gridUnit * 40
    width: Kirigami.Units.gridUnit * 45
    height: Kirigami.Units.gridUnit * 35

    pageStack.initialPage: Qt.createComponent(mainPageComponent)

    Connections {
        target: AppController
        function onShowWindow() {
            root.show()
            root.raise()
        }

        function onShowSettings() {
            root.show()
            root.raise()
            root.pageStack.pushDialogLayer(settingsPageComponent, {}, {
                                               "minimumWidth": Kirigami.Units.gridUnit * 30,
                                               "minimumHeight": Kirigami.Units.gridUnit * 20,
                                               "maximumWidth": Kirigami.Units.gridUnit * 50,
                                               "maximumHeight": Kirigami.Units.gridUnit * 40,
                                               "width": Kirigami.Units.gridUnit * 40,
                                               "height": Kirigami.Units.gridUnit * 30
                                           })
        }
    }

    Component.onCompleted: {
        if (AppController.startMinimized) {
            root.close()
        }
    }
}
