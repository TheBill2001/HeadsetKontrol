import QtQuick
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.settings as KirigamiSettings

import com.gitlab.thebill2001.headsetkontrol

KirigamiSettings.ConfigurationView {
    id: settingsView

    property var shortcutModel

    modules: [
        KirigamiSettings.ConfigurationModule {
            moduleId: "general"
            text: i18nc("@action:inmenu", "General")
            icon.name: "configure"
            page: () => Qt.createComponent("com.gitlab.thebill2001.headsetkontrol", "GeneralSettingsPage")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "headsetcontrol"
            text: i18nc("@action:inmenu", "HeadsetControl")
            icon.name: "headsetkontrol"
            page: () => Qt.createComponent("com.gitlab.thebill2001.headsetkontrol", "HeadsetControlSettingsPage")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "shortcuts"
            text: i18nc("@action:button", "Shortcuts")
            icon.name: "configure-shortcuts"
            page: () => Qt.createComponent("org.kde.kirigamiaddons.statefulapp.private", 'ShortcutsEditor')
            initialProperties: () => ({
                        model: settingsView.shortcutModel
                    })
            category: i18nc("@title:group", "Shortcuts")
        },
        KirigamiSettings.ConfigurationModule {
            moduleId: "about"
            text: i18nc("@action:inmenu", "About")
            icon.name: "help-about"
            page: () => Qt.createComponent("com.gitlab.thebill2001.headsetkontrol", "AboutSettingsPage")
        }
    ]

    // Override default behaviour
    function open(defaultModule = '') {
        const component = Qt.createComponent('org.kde.kirigamiaddons.settings.private', 'ConfigWindow');
        if (component.status === Component.Failed) {
            console.error(component.errorString());
            return;
        }

        root.restore();

        component.createObject(root, {
            "modality": Qt.ApplicationModal,
            "defaultModule": defaultModule,
            "modules": settingsView.modules,
            "width": Kirigami.Units.gridUnit * 50,
            "height": Kirigami.Units.gridUnit * 30,
            "minimumWidth": Kirigami.Units.gridUnit * 50,
            "minimumHeight": Kirigami.Units.gridUnit * 30,
            "title": settingsView.title
        });
    }
}
