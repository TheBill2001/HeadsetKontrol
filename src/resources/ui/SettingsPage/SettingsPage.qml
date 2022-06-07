import org.kde.kirigami 2.19 as Kirigami

Kirigami.CategorizedSettings {
    id: settingPage
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
}
