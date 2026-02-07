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

        onCurrentHeadsetChanged: {
            if (currentHeadset) {
                if (root.pageStack.initialPage instanceof HeadsetPage) {
                    (root.pageStack.initialPage as HeadsetPage).headset = currentHeadset;
                } else {
                    root.pageStack.initialPage = headsetPageComponent.createObject(null, {
                        headset: currentHeadset
                    });
                }
            } else {
                root.pageStack.initialPage = welcomePageComponent.createObject(null);
            }
        }
    }

    pageStack.initialPage: welcomePageComponent.createObject(null)

    Component {
        id: headsetPageComponent

        HeadsetPage {}
    }

    Component {
        id: welcomePageComponent

        Kirigami.Page {
            Kirigami.Theme.colorSet: Kirigami.Theme.View

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Item {
                    Layout.fillHeight: true
                }

                Kirigami.Icon {
                    source: "headsetkontrol"

                    implicitWidth: Kirigami.Units.iconSizes.huge
                    implicitHeight: Kirigami.Units.iconSizes.huge

                    Layout.preferredWidth: implicitWidth
                    Layout.preferredHeight: implicitHeight
                    Layout.alignment: Qt.AlignHCenter
                }

                Kirigami.Heading {
                    text: KI18n.i18nc("@title", "HeadsetKontrol")
                    type: Kirigami.Heading.Primary

                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: Kirigami.Units.largeSpacing
                }

                Kirigami.Heading {
                    text: KI18n.i18nc("@info:placeholder", "No headset selected.")
                    type: Kirigami.Heading.Secondary
                    level: 3

                    Layout.alignment: Qt.AlignHCenter
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
