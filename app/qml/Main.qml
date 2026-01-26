// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick
import QtQuick.Layouts

import org.kde.ki18n
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.statefulapp as StatefulApp
import org.kde.kirigamiaddons.settings as Settings

StatefulApp.StatefulWindow {
    id: root

    title: KI18n.i18nc("@title:window", "Control Your Headsets")

    windowName: "HeadsetKontrolMainWindow"

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    application: HKApplication {
        id: hkApp

        window: root

        configurationView: Settings.ConfigurationView {
            window: root

            modules: [
                Settings.ConfigurationModule {
                    id: generalConfigModule

                    icon.name: "configure"
                    initialProperties: () => ({
                                title: Qt.binding(() => generalConfigModule.text)
                            })
                    moduleId: "general"
                    page: () => Qt.createComponent("com.thebill2001.headsetkontrol", "GeneralConfigPage")
                    text: KI18n.i18nc("@title:tab", "General")
                },
                Settings.ConfigurationModule {
                    id: userInterfaceConfigModule

                    icon.name: "interface"
                    initialProperties: () => ({
                                title: Qt.binding(() => userInterfaceConfigModule.text)
                            })
                    moduleId: "user-interface"
                    page: () => Qt.createComponent("com.thebill2001.headsetkontrol", "UserInterfaceConfigPage")
                    text: KI18n.i18nc("@title:tab", "User Interface")
                },
                Settings.ConfigurationModule {
                    id: headsetcontrolConfigModule

                    icon.name: "audio-headset"
                    initialProperties: () => ({
                                title: Qt.binding(() => headsetcontrolConfigModule.text)
                            })
                    moduleId: "headsetcontrol"
                    page: () => Qt.createComponent("com.thebill2001.headsetkontrol", "HeadsetControlConfigPage")
                    text: KI18n.i18nc("@title:tab", "Control")
                },
                Settings.ShortcutsConfigurationModule {
                    application: hkApp
                },
                Settings.ConfigurationModule {
                    readonly property Kirigami.Action action: Kirigami.Action {
                        fromQAction: hkApp.aboutAppAction
                    }

                    icon.name: action.icon.name
                    icon.source: action.icon.source
                    moduleId: "about"
                    page: () => Qt.createComponent("org.kde.kirigamiaddons.formcard", "AboutPage")
                    text: action.text
                }
            ]
        }
    }

    Kirigami.Action {
        fromQAction: hkApp.commandBarAction
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    globalDrawer: AppDrawer {
        id: appDrawer
    }

    pageStack.initialPage: Kirigami.Page {
        Kirigami.Theme.colorSet: Kirigami.Theme.View

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent

            width: parent.width - (Kirigami.Units.largeSpacing * 4)
            visible: true

            icon.name: "headsetkontrol"
            icon.width: Kirigami.Units.iconSizes.huge
            icon.height: Kirigami.Units.iconSizes.huge

            text: KI18n.i18nc("@title", "No Headset Selected")

            Layout.alignment: Qt.AlignHCenter
        }
    }

    Component.onCompleted: HKHeadsetControl.start()
}
