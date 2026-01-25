// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.ki18n
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.statefulapp as StatefulApp

Kirigami.OverlayDrawer {
    id: drawer

    property StatefulApp.StatefulWindow window: StatefulApp.StatefulWindow.window as StatefulApp.StatefulWindow
    property HKApplication application: window.application as HKApplication

    edge: Application.layoutDirection === Qt.RightToLeft ? Qt.RightEdge : Qt.LeftEdge
    modal: Kirigami.Settings.isMobile || (window.width < Kirigami.Units.gridUnit * 50 && !collapsed) // Only modal when not collapsed, otherwise collapsed won't show.
    width: Kirigami.Units.gridUnit * 16
    onModalChanged: drawerOpen = !modal

    Kirigami.Theme.colorSet: Kirigami.Theme.Window

    handleClosedIcon.name: "application-menu"
    handleOpenIcon.name: "tab-close"
    handleVisible: modal

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    contentItem: ColumnLayout {
        spacing: 0

        QQC2.ToolBar {
            Layout.fillWidth: true
            Layout.preferredHeight: drawer.window.pageStack.globalToolBar.preferredHeight
            position: QQC2.ToolBar.Header

            contentItem: RowLayout {
                spacing: 0

                Kirigami.Heading {
                    text: KI18n.i18nc("@title", "Headsets")
                    elide: Qt.ElideRight

                    Layout.fillWidth: true
                    Layout.leftMargin: Kirigami.Units.largeSpacing
                }

                Item {
                    Layout.fillWidth: true
                }

                QQC2.ToolButton {
                    action: Kirigami.Action {
                        fromQAction: drawer.application.refreshAction
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        QQC2.ToolBar {
            Layout.fillWidth: true
            Layout.preferredHeight: drawer.window.pageStack.globalToolBar.preferredHeight
            position: QQC2.ToolBar.Footer

            contentItem: RowLayout {
                spacing: Kirigami.Units.smallSpacing

                QQC2.ToolButton {
                    action: Kirigami.Action {
                        fromQAction: drawer.application.startAction
                    }

                    display: HKConfig.showRefreshTimer ? QQC2.ToolButton.IconOnly : QQC2.ToolButton.TextBesideIcon
                    visible: enabled

                    Kirigami.MnemonicData.label: text

                    QQC2.ToolTip.text: Kirigami.MnemonicData.richTextLabel
                    QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                    QQC2.ToolTip.visible: hovered
                }

                QQC2.ToolButton {
                    action: Kirigami.Action {
                        fromQAction: drawer.application.stopAction
                    }

                    display: HKConfig.showRefreshTimer ? QQC2.ToolButton.IconOnly : QQC2.ToolButton.TextBesideIcon
                    visible: enabled

                    Kirigami.MnemonicData.label: text

                    QQC2.ToolTip.text: Kirigami.MnemonicData.richTextLabel
                    QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                    QQC2.ToolTip.visible: hovered
                }

                QQC2.Label {
                    id: refreshTimerLabel

                    visible: HKConfig.showRefreshTimer

                    Timer {
                        interval: 100
                        repeat: true
                        running: refreshTimerLabel.visible && HKHeadsetControl.isRunning && drawer.window.visible && !(Window.visibility & (Window.Hidden | Window.Minimized)) && !(Application.state & (Qt.ApplicationHidden | Qt.ApplicationSuspended))
                        triggeredOnStart: true

                        onTriggered: refreshTimerLabel.text = refreshTimerLabel.timeToText(HKHeadsetControl.timeUntilRefresh())
                    }

                    Connections {
                        target: HKHeadsetControl

                        function onRunningChanged(running: bool) {
                            if (!running) {
                                refreshTimerLabel.text = refreshTimerLabel.timeToText(0);
                            }
                        }
                    }

                    Component.onCompleted: text = timeToText(0)

                    function timeToText(time: int): string {
                        const clamppedTime = Math.max(0, time);

                        const hours = Math.floor(clamppedTime / 3600000);
                        let reminder = clamppedTime % 3600000;

                        const minutes = Math.floor(reminder / 60000);
                        reminder = reminder % 60000;

                        const seconds = Math.floor(reminder / 1000);
                        const milliseconds = reminder % 1000;

                        let str = `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}.${String(milliseconds).padStart(3, '0')}`;
                        if (hours > 0) {
                            str = `${String(hours).padStart(2, '0')}:${str}`;
                        }

                        return str;
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                QQC2.ToolButton {
                    action: Kirigami.Action {
                        fromQAction: drawer.application.configureAction
                    }

                    display: QQC2.ToolButton.IconOnly
                    visible: enabled

                    Kirigami.MnemonicData.label: text

                    QQC2.ToolTip.text: Kirigami.MnemonicData.richTextLabel
                    QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                    QQC2.ToolTip.visible: hovered
                }

                QQC2.ToolButton {
                    action: Kirigami.Action {
                        fromQAction: drawer.application.quitAction
                    }

                    display: QQC2.ToolButton.IconOnly
                    visible: enabled

                    Kirigami.MnemonicData.label: text

                    QQC2.ToolTip.text: Kirigami.MnemonicData.richTextLabel
                    QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                    QQC2.ToolTip.visible: hovered
                }
            }
        }
    }
}
