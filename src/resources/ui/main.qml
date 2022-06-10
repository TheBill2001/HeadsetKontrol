import QtQuick 2.15
import org.kde.kirigami 2.19 as Kirigami

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Control your headset")

    minimumWidth: Kirigami.Units.gridUnit * 30
    minimumHeight: Kirigami.Units.gridUnit * 20
    maximumWidth: Kirigami.Units.gridUnit * 50
    maximumHeight: Kirigami.Units.gridUnit * 40
    width: Kirigami.Units.gridUnit * 40
    height: Kirigami.Units.gridUnit * 30

    pageStack.initialPage: MainPage{}
}

