import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

import headsetkontrol 1.0

Kirigami.AboutPage {
    id: aboutSettingsPage
    title: i18nc('@title:window', 'About')
    aboutData: AboutType.aboutData
}
