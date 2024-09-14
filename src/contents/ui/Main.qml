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
        id: globalDrawer

        isMenu: true

        actions: [
            Kirigami.Action {
                fromQAction: App.aboutAction
            },
            Kirigami.Action {
                fromQAction: App.reportBugAction
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                fromQAction: App.configureKeyBindingsAction
            },
            Kirigami.Action {
                fromQAction: App.configureAction
            },
            Kirigami.Action {
                fromQAction: App.configureNotificationsAction
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                fromQAction: App.quitAction
            }
        ]
    }

    pageStack.initialPage: DevicesPage {
        id: devicesPage
    }

    onVisibleChanged: {
        if (visible)
            HeadsetControl.startCountDownUpdateTimer()
        else
            HeadsetControl.stopCountDownUpdateTimer()
    }

    Component.onCompleted: {
        if (Config.runInBackground) {
            root.visible = !Config.startMinimized
        } else {
            root.visible = true
        }
    }

    SettingsView {
        id: settingsView

        window: root
    }

    Connections {
        target: App
        function onShowWindow() {
            root.restore()
        }

        function onShowSettings() {
            root.restore()
            root.openSettingsPage()
        }

        function onShowAbout() {
            root.restore()
            root.openSettingsPage("about")
        }

        function onShowDevice(device) {
            root.restore()
            root.openDeviceDetailPage(device)
        }
    }

    DeviceDetailPage {
        id: deviceDetailPage

        device: root.selectedDevice
    }

    function openSettingsPage(module = null) {
        settingsView.open(module ? module : '')
    }

    function openDeviceDetailPage(device) {
        root.selectedDevice = device

        while (root.pageStack.depth > 1) {
            root.pageStack.pop()
        }

        root.pageStack.push(deviceDetailPage)
    }

    function restore() {
        root.show()
        root.raise()
    }
}
