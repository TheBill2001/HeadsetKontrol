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
            text: i18nc("@option:check", "Run in background")
            checked: Config.runInBackground
            onCheckedChanged: Config.runInBackground = checked
        }

        KirigamiFormCard.FormSwitchDelegate {
            text: i18nc("@option:check", "Start minimized")
            enabled: Config.runInBackground
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
}
