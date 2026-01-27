// SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

import QtQuick
import QtQuick.Controls as QQC2
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.AbstractFormDelegate {
    id: root

    property alias from: internalProgressBar.from
    property alias indeterminate: internalProgressBar.indeterminate
    property bool showValue: true
    property alias to: internalProgressBar.to
    property alias value: internalProgressBar.value
    property string suffix

    background: null
    hoverEnabled: false

    contentItem: ColumnLayout {
        spacing: 0

        QQC2.Label {
            Accessible.ignored: true
            Layout.fillWidth: true
            elide: Text.ElideRight
            text: root.text
        }

        RowLayout {
            spacing: Kirigami.Units.smallSpacing

            QQC2.ProgressBar {
                id: internalProgressBar

                Layout.fillWidth: true
            }

            QQC2.Label {
                text: `${internalProgressBar.value}${root.suffix}`
                visible: root.showValue
            }
        }
    }
}
