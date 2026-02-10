// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick

import org.kde.ki18n
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: root

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "User Interface")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: showRefreshTimerDelegate

            checked: HKConfig.showRefreshTimer
            description: KI18n.i18nc("@info:usagetip", "Show the countdown timer until the next refresh. This option can cause high CPU usage.")
            text: KI18n.i18nc("@option:check", "Show refresh timer")

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.showRefreshTimer = checked;
                HKConfig.save();
            })
        }
    }
}
