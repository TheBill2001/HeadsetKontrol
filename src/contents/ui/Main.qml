import QtQuick
import org.kde.kirigami as Kirigami

import com.gitlab.thebill2001.headsetkontrol

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Control your headset")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 10

    width: Kirigami.Units.gridUnit * 45
    height: Kirigami.Units.gridUnit * 35

    property Device selectedDevice: null

    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: true

        actions: [
            Kirigami.Action {
                id: settingsAction
                icon.name: "configure"
                text: i18nc("@action:button", "Settings")
                onTriggered: root.openSettingsPage()
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                id: quitAction
                icon.name: "application-exit"
                text: i18nc("@action:button", "Quit")
                onTriggered: Qt.quit()
            }
        ]
    }

    pageStack.initialPage: DevicesPage {
        id: devicesPage
    }

    onVisibleChanged: visible ? HeadsetControl.stopCountDownUpdateTimer(
                                    ) : HeadsetControl.startCountDownUpdateTimer()

    SettingsView {
        id: settingsView

        window: root
    }

    Connections {
        target: TrayIcon
        function onShowWindow() {
            root.show()
            root.raise()
        }

        function onShowSettings() {
            root.show()
            root.raise()
            root.openSettingsPage()
        }
    }

    DeviceDetailPage {
        id: deviceDetailPage

        device: root.selectedDevice
    }

    // Component.onCompleted: {
    //     if (AppController.startMinimized) {
    //         root.close()
    //     }
    // }
    function openSettingsPage() {
        settingsView.open()
    }

    function openDeviceDetailPage(device) {
        root.selectedDevice = device

        while (root.pageStack.depth > 1) {
            root.pageStack.pop()
        }

        root.pageStack.push(deviceDetailPage)
    }
}
