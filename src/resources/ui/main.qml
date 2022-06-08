import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Hello World")

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
    }
}

