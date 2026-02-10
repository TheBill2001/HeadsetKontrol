// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

Kirigami.PromptDialog {
    id: root

    required property hkError error

    dialogType: Kirigami.PromptDialog.Error
    title: error.title
    preferredHeight: Kirigami.Units.gridUnit * 16

    onClosed: destroy()

    ColumnLayout {
        spacing: Kirigami.Units.largeSpacing

        Kirigami.SelectableLabel {
            Layout.fillWidth: true

            text: root.error.message
        }

        QQC2.TextArea {
            Layout.fillWidth: true

            text: root.error.details
            visible: text.length > 0
        }
    }
}
