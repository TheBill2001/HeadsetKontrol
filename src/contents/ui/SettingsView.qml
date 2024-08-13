import QtQuick
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.settings as KirigamiSettings

import com.gitlab.thebill2001.headsetkontrol

KirigamiSettings.ConfigurationView {
    id: settingsView

    modules: [
        KirigamiSettings.ConfigurationModule {
            moduleId: "general"
            text: i18nc("@action:button", "General")
            icon.name: "configure"
            page: () => Qt.createComponent(
                      "com.gitlab.thebill2001.headsetkontrol",
                      "GeneralSettingsPage")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "headsetcontrol"
            text: i18nc("@action:button", "HeadsetControl")
            icon.name: "headsetkontrol"
            page: () => Qt.createComponent(
                      "com.gitlab.thebill2001.headsetkontrol",
                      "HeadsetControlSettingsPage")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "about"
            text: i18nc("@action:button", "About")
            icon.name: "help-about"
            page: () => Qt.createComponent(
                      "com.gitlab.thebill2001.headsetkontrol",
                      "AboutSettingsPage")
        }
    ]

    Component.onDestruction: Config.save()
}
