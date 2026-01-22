// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.ki18n
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.statefulapp as StatefulApp

StatefulApp.StatefulWindow {
    id: root

    title: KI18n.i18nc("@title:window", "Control Your Headsets")

    windowName: "HeadsetKontrolMainWindow"

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    application: HKApplication {
        id: hkApp
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    globalDrawer: Kirigami.OverlayDrawer {
        id: drawer
        edge: Application.layoutDirection === Qt.RightToLeft ? Qt.RightEdge : Qt.LeftEdge
        modal: Kirigami.Settings.isMobile || (root.width < Kirigami.Units.gridUnit * 50 && !collapsed) // Only modal when not collapsed, otherwise collapsed won't show.
        width: Kirigami.Units.gridUnit * 16
        onModalChanged: drawerOpen = !modal

        Behavior on width {
            NumberAnimation {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        Kirigami.Theme.colorSet: Kirigami.Theme.Window

        handleClosedIcon.source: modal ? null : "sidebar-expand-left"
        handleOpenIcon.source: modal ? null : "sidebar-collapse-left"
        handleVisible: modal

        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        contentItem: ColumnLayout {
            spacing: 0
            QQC2.ToolBar {
                Layout.fillWidth: true
                Layout.preferredHeight: root.pageStack.globalToolBar.preferredHeight

                contentItem: RowLayout {
                    spacing: 0

                    Kirigami.Heading {
                        text: KI18n.i18nc("@title", "Headsets")
                        elide: Qt.ElideRight

                        Layout.fillWidth: true
                        Layout.leftMargin: Kirigami.Units.largeSpacing
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                // Layout.fillWidth: true
            }
        }
    }

    pageStack.initialPage: Kirigami.Page {
        title: "Hello, world!"
    }
}
