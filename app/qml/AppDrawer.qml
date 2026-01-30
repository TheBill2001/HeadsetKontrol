// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

pragma ComponentBehavior: Bound

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
    property HKHeadset currentHeadset: null

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

        QQC2.ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: listView

                clip: true
                activeFocusOnTab: true
                keyNavigationEnabled: true
                Accessible.role: Accessible.List

                onCurrentItemChanged: drawer.currentHeadset = (currentItem as HeadsetItemDelegate)?.headset ?? null

                model: HKHeadsetListModel {}

                delegate: HeadsetItemDelegate {}

                Kirigami.PlaceholderMessage {
                    anchors.centerIn: parent
                    visible: listView.count <= 0

                    text: KI18n.i18nc("@info:placeholder", "No headset found.")
                }

                QQC2.Menu {
                    id: contextMenu

                    readonly property HeadsetItemDelegate triggerItem: parent as HeadsetItemDelegate

                    QQC2.MenuItem {
                        action: Kirigami.Action {
                            enabled: contextMenu.triggerItem ? (String(contextMenu.triggerItem.headsetId) !== HKConfig.primaryDevice) : false
                            icon.name: "favorite-favorited"
                            text: KI18n.i18nc("@action:inmenu", "Set as primary device")

                            onTriggered: {
                                if (contextMenu.triggerItem) {
                                    HKConfig.primaryDevice = String(contextMenu.triggerItem.headsetId);
                                }
                            }
                        }
                    }

                    QQC2.MenuItem {
                        action: Kirigami.Action {
                            icon.name: "dialog-ok-apply"
                            text: KI18n.i18nc("@action:inmenu", "Apply device settings")

                            // TODO
                            onTriggered: {}
                        }
                    }
                }
            }
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

    component HeadsetItemDelegate: QQC2.ItemDelegate {
        id: delegate

        required property int index
        required property HKHeadset headset
        required property hkHeadsetId headsetId
        required property string headsetName
        required property int headsetCapabilities
        required property hkBattery headsetBattery
        required property hkChatMix headsetChatMix

        text: Kirigami.MnemonicData.richTextLabel
        width: ListView.view.width
        highlighted: ListView.isCurrentItem

        QQC2.ContextMenu.menu: contextMenu

        Kirigami.MnemonicData.label: headsetName
        Kirigami.MnemonicData.enabled: enabled && visible
        Kirigami.MnemonicData.controlType: Kirigami.MnemonicData.MenuItem

        Accessible.name: Kirigami.MnemonicData.plainTextLabel
        Accessible.role: Accessible.PageTab
        Accessible.description: QQC2.ToolTip.text

        QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
        QQC2.ToolTip.visible: hovered
        QQC2.ToolTip.text: {
            let text = delegate.headsetName + `<ul><li>${KI18n.i18nc("@item:intext headset ID", "ID: %1", String(delegate.headsetId))}</li>`;
            if (delegate.headsetCapabilities & HKHeadset.BatteryStatusCapability) {
                if (delegate.headsetBattery.status <= 0) {
                    text += `<li>${KI18n.i18nc("@item:intext", "Battery: %1", HKUtils.batteryStatusToLocaleString(delegate.headsetBattery.status))}</li>`;
                } else if (delegate.headsetBattery.status === HKBattery.BatteryCharging) {
                    text += `<li>${KI18n.i18nc("@item:intext", "Battery: %1% (%2)", delegate.headsetBattery.level, HKUtils.batteryStatusToLocaleString(delegate.headsetBattery.status))}</li>`;
                } else {
                    text += `<li>${KI18n.i18nc("@item:intext", "Battery: %1%", delegate.headsetBattery.level)}</li>`;
                }
            }
            if (delegate.headsetCapabilities & HKHeadset.ChatMixStatusCapability) {
                text += `<li>${KI18n.i18nc("@item:intext", "ChatMix: %1", delegate.headsetChatMix.level)}</li>`;
            }
            return text + '</ul>';
        }

        contentItem: RowLayout {
            spacing: Kirigami.Units.mediumSpacing

            Kirigami.IconTitleSubtitle {
                id: iconTitleSubtitle

                Layout.fillWidth: true

                icon.name: delegate.headsetId == HKConfig.primaryDevice ? "favorite-favorited" : "" // qmllint disable equality-type-coercion
                selected: delegate.highlighted || delegate.down
                title: delegate.text
                subtitle: String(delegate.headsetId)
            }

            Kirigami.Icon {
                id: batteryIcon

                color: iconTitleSubtitle.icon.color
                implicitHeight: iconTitleSubtitle.icon.height
                implicitWidth: iconTitleSubtitle.icon.width
                selected: iconTitleSubtitle.selected
                source: HKUtils.batteryIconName(delegate.headsetBattery)

                Layout.preferredHeight: implicitHeight
                Layout.preferredWidth: implicitWidth
            }
        }

        onClicked: ListView.view.currentIndex = index
    }
}
