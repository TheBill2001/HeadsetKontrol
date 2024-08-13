import org.kde.kirigamiaddons.formcard as KirigamiFormCard

import com.gitlab.thebill2001.headsetkontrol

KirigamiFormCard.FormCardPage {
    id: generalSettingsPage

    title: i18nc('@title:page', 'General')

    KirigamiFormCard.FormHeader {
        title: i18n("General")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSwitchDelegate {
            text: i18n("Run in backgroud")
            checked: Config.runInBackground
            onCheckedChanged: Config.runInBackground = checked
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18n("Autostart")
            checked: Config.autoStart
            onCheckedChanged: Config.autoStart = checked
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18n("User interface")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSectionText {
            text: i18n("Can cause high CPU usage")
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18n("Show countdown progress")
            checked: Config.showCountdownProgress
            onCheckedChanged: Config.showCountdownProgress = checked
        }
    }

    KirigamiFormCard.FormHeader {
        title: i18n("Notifications")
    }

    KirigamiFormCard.FormCard {
        KirigamiFormCard.FormSwitchDelegate {
            text: i18n("Battery notification")
            checked: Config.batteryNotification
            onCheckedChanged: Config.batteryNotification = checked
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18n("Device change notification")
            checked: Config.deviceChangeNotification
            onCheckedChanged: Config.deviceChangeNotification = checked
        }
    }
}
