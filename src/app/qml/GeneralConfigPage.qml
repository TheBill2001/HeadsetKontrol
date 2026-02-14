// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick

import org.kde.ki18n
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormCardPage {
    id: root

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Startup")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: autoStartDelegate

            checked: HKConfig.autoStart
            description: KI18n.i18nc("@info:usagetip", "Start the application automatically after logging in your desktop environment.")
            text: KI18n.i18nc("@option:check", "Auto-start")
            enabled: !HKConfig.isAutoStartImmutable

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.autoStart = checked;
                HKConfig.save();
            })
        }

        FormCard.FormSwitchDelegate {
            id: startMinizedDelegate

            checked: HKConfig.startMinimized
            description: KI18n.i18nc("@info:usagetip", "Start the application minimized.")
            text: KI18n.i18nc("@option:check", "Start minimized")
            enabled: !HKConfig.isStartMinimizedImmutable

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.startMinimized = checked;
                HKConfig.save();
            })
        }
    }

    FormCard.FormHeader {
        title: KI18n.i18nc("@title:group", "Behaviour")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: useTrayIconDelegate

            checked: HKConfig.useTrayIcon
            description: KI18n.i18nc("@info:usagetip", "Display an application icon in system tray if possible. This option does not control whether the application will run in the background.")
            text: KI18n.i18nc("@option:check", "Use tray icon")
            enabled: !HKConfig.isUseTrayIconImmutable

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.useTrayIcon = checked;
                HKConfig.save();
            })
        }

        FormCard.FormSwitchDelegate {
            id: runInBackgroundDelegate

            checked: HKConfig.runInBackground
            description: KI18n.i18nc("@info:usagetip", "Keeping the application running in the background after closing the main window. It is recommended to enable tray icon along with this option.")
            text: KI18n.i18nc("@option:check", "Run in background")
            enabled: !HKConfig.isRunInBackgroundImmutable

            onCheckedChanged: Qt.callLater(() => {
                HKConfig.runInBackground = checked;
                HKConfig.save();
            })
        }
    }
}
