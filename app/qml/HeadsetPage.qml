// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick
import QtQuick.Layouts

import org.kde.ki18n
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: root

    required property HKHeadset headset

    function hasCapability(cap: int): bool {
        return headset ? (headset.capabilities & cap) : false;
    }

    title: headset?.name ?? ""

    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            headset?.refresh();
        }
    }

    Connections {
        target: root.headset

        function onRefreshDone() {
            root.refreshing = false;
        }
    }

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Information")
    }

    FormCard.FormGridContainer {
        infoCards: [
            FormCard.FormGridContainer.InfoCard {
                title: root.headset?.name ?? ""
                subtitle: KI18n.i18nc("@label:textbox", "Headset name")
            },
            FormCard.FormGridContainer.InfoCard {
                title: root.headset ? String(root.headset.id) : ""
                subtitle: KI18n.i18nc("@label:textbox", "Headset ID")
            }
        ]

        Layout.bottomMargin: Kirigami.Units.smallSpacing
    }

    FormCard.FormCard {
        FormCard.FormButtonDelegate {
            text: KI18n.i18ncp("@lable:textbox", "%1 capability", "%1 capabilities", root.headset?.capabilityCount ?? 0)
            description: KI18n.i18nc("@action:button", "View capabilities")

            onClicked: capabilitiesOverlaySheet.open()
        }

        Kirigami.OverlaySheet {
            id: capabilitiesOverlaySheet

            title: KI18n.i18nc("@title:window", "Capabilities")
            parent: root

            ListView {
                model: root.headset?.capabilitiesLocaleStrings ?? []
                implicitWidth: Kirigami.Units.gridUnit * 20

                spacing: Kirigami.Units.smallSpacing

                delegate: Kirigami.SelectableLabel {
                    required property int index
                    required property string modelData

                    leftPadding: Kirigami.Units.largeSpacing
                    rightPadding: leftPadding
                    topPadding: index === 0 ? leftPadding : 0
                    bottomPadding: index === ListView.view.count - 1 ? leftPadding : 0

                    text: modelData
                }
            }
        }
    }

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Battery")
        visible: batteryFromCard.visible
    }

    FormCard.FormCard {
        id: batteryFromCard

        visible: root.hasCapability(HKHeadset.BatteryStatusCapability)

        FormProgressBarDelegate {
            from: 0
            suffix: "%"
            text: KI18n.i18nc("@label", "Battery level")
            to: 100
            value: root.headset?.battery.level ?? 0
            visible: root.headset ? (root.headset.battery.status > HKBattery.BatteryUnavailable) : false
        }

        Kirigami.InlineMessage {
            Layout.bottomMargin: Kirigami.Units.largeSpacing
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing

            text: root.headset ? root.headset.battery.batteryStatusToLocaleString() : ""
            type: {
                if (root.headset) {
                    if (root.headset.battery.status < HKBattery.BatteryUnavailable) {
                        return Kirigami.MessageType.Error;
                    } else if (root.headset.battery.status === HKBattery.BatteryUnavailable) {
                        return Kirigami.MessageType.Warning;
                    } else if (root.headset.battery.status === HKBattery.BatteryCharging) {
                        return Kirigami.MessageType.Positive;
                    }
                }
                return Kirigami.MessageType.Information;
            }
            visible: root.headset?.battery.status !== HKBattery.BatteryAvailable
        }
    }

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "ChatMix")
        visible: chatMixFormCard.visible
    }

    FormCard.FormCard {
        id: chatMixFormCard

        visible: root.hasCapability(HKHeadset.ChatMixStatusCapability)

        FormProgressBarDelegate {
            from: 0
            text: KI18n.i18nc("@label", "ChatMix level")
            to: 128
            value: root.headset?.chatMix.level ?? 0
        }

        FormProgressBarDelegate {
            from: 0
            text: KI18n.i18nc("@label", "Game volume")
            to: 100
            value: root.headset?.chatMix.gameVolumePercent ?? 0
            suffix: '%'
        }

        FormProgressBarDelegate {
            from: 0
            text: KI18n.i18nc("@label", "Chat volume")
            to: 100
            value: root.headset?.chatMix.chatVolumePercent ?? 0
            suffix: '%'
        }
    }
}
