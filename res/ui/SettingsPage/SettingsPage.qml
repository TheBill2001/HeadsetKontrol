import QtQuick 2.15
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

Kirigami.CategorizedSettings {
    id: settingsPage
    actions: [
        Kirigami.SettingAction {
            text: i18n("General")
            icon.name: "settings"
            page: Qt.resolvedUrl("GeneralSettingsPage.qml")
        },
        Kirigami.SettingAction {
            text: i18n("Notification")
            icon.name: "notifications"
            page: Qt.resolvedUrl("NotificationSettingsPage.qml")
        },
        Kirigami.SettingAction {
            text: i18n("About")
            icon.name: "help-about"
            page: Qt.resolvedUrl("AboutSettingsPage.qml")
        }
    ]

    Component.onDestruction: {
        AppController.config.save()
    }
}
