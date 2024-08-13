import org.kde.kirigamiaddons.formcard as KirigamiFormCard

import com.gitlab.thebill2001.headsetkontrol

KirigamiFormCard.AboutPage {
    id: aboutSettingsPage
    title: i18nc('@title:window', 'About')
    aboutData: Config.aboutData
}
