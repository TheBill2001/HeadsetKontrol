import org.kde.kirigamiaddons.formcard as KirigamiFormCard

import com.gitlab.thebill2001.headsetkontrol

KirigamiFormCard.FormCardPage {
    id: generalSettingsPage

    title: i18nc('@title:tab', 'General')

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "General")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSwitchDelegate {
            text: i18nc("@option:check", "Start minimized")
            checked: Config.startMinimized
            onCheckedChanged: Config.startMinimized = checked
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18nc("@option:check", "Autostart")
            checked: Config.autoStart
            onCheckedChanged: Config.autoStart = checked
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "User interface")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSectionText {
            text: i18nc("@info:usagetip", "Can cause high CPU usage")
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18nc("@option:check", "Show countdown progress")
            checked: Config.showCountdownProgress
            onCheckedChanged: Config.showCountdownProgress = checked
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18nc("@title:group", "Notifications")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSwitchDelegate {
            text: i18nc("@option:check", "Battery notification")
            checked: Config.batteryNotification
            onCheckedChanged: Config.batteryNotification = checked
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18nc("@option:check", "Device change notification")
            checked: Config.deviceChangeNotification
            onCheckedChanged: Config.deviceChangeNotification = checked
        }
    }
}
