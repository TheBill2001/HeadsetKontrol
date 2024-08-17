import QtQuick.Layouts
import QtQuick.Controls as Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as KirigamiFormCard

KirigamiFormCard.AbstractFormDelegate {
    id: sliderDelegate

    property alias value: slider.value
    property alias from: slider.from
    property alias to: slider.to
    property alias stepSize: slider.stepSize

    signal moved

    hoverEnabled: false
    background: null

    contentItem: ColumnLayout {
        spacing: Kirigami.Units.smallSpacing

        Controls.Label {
            Layout.fillWidth: true

            text: sliderDelegate.text
            visible: sliderDelegate.text
        }

        RowLayout {
            Controls.Slider {
                id: slider
                Layout.fillWidth: true

                snapMode: Controls.Slider.SnapAlways

                onMoved: sliderDelegate.moved()
            }

            Controls.Label {
                Layout.leftMargin: Kirigami.Units.smallSpacing
                Layout.preferredWidth: Kirigami.Units.gridUnit

                horizontalAlignment: Controls.Label.AlignRight
                text: slider.value
            }
        }
    }
}
