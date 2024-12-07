import QtQuick
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.statefulapp as StatefulApplication

import com.gitlab.thebill2001.headsetkontrol

StatefulApplication.StatefulWindow {
    id: root

    objectName: "root"

    windowName: i18nc("@title:window", "Control your headset")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 10

    property Device selectedDevice: null

    application: App {
        id: app

        configurationView: SettingsView {
            id: settingsView

            shortcutModel: app.shortcutsModel
        }
    }

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer

        isMenu: true

        actions: [
            Kirigami.Action {
                fromQAction: app.action(App.AboutApp)
            },
            Kirigami.Action {
                fromQAction: app.action(App.ReportBug)
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                fromQAction: app.action(App.KeyBindings)
            },
            Kirigami.Action {
                fromQAction: app.action(App.Preferences)
            },
            Kirigami.Action {
                fromQAction: app.action(App.ConfigureNotifications)
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                fromQAction: app.action(App.Quit)
            }
        ]
    }

    pageStack.initialPage: DevicesPage {
        id: devicesPage

        app: app
    }

    onVisibleChanged: {
        if (visible)
            HeadsetControl.startCountDownUpdateTimer();
        else
            HeadsetControl.stopCountDownUpdateTimer();
    }

    Component.onCompleted: {
        if (Config.runInBackground) {
            root.visible = !Config.startMinimized;
        } else {
            root.visible = true;
        }
    }

    Connections {
        target: app

        function onShowWindow() {
            root.restore();
        }

        function onShowDevice(device) {
            root.restore();
            root.openDeviceDetailPage(device);
        }
    }

    DeviceDetailPage {
        id: deviceDetailPage

        device: root.selectedDevice
    }

    function openSettingsPage(module = null) {
        settingsView.open(module ? module : '');
    }

    function openDeviceDetailPage(device) {
        root.selectedDevice = device;

        while (root.pageStack.depth > 1) {
            root.pageStack.pop();
        }

        root.pageStack.push(deviceDetailPage);
    }

    function restore() {
        root.show();
        root.raise();
    }
}
