import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    property real temperature: 0
    property real temperatureF: 0
    ColumnLayout {
        spacing: 20

        Text {
            text: "Temperature"
            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            width: 120
            height: 120

            // Create a thicker border to simulate the Dial
            border.color: "crimson"
            border.width: 10
            radius: width / 2

            Text {
                id: temperatureLabel
                text: temperatureDial.value.toFixed(1) + "°C"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }
        }

        Dial {
            id: temperatureDial
            from: -20
            visible: false
            to: 40
            value: temperature
            stepSize: 0.01
            enabled: false // Making the dial read-only
            width: 0
            height: 0 // Hide the dial, we're using the Rectangle above as the visual representation

            onValueChanged: {
                // Update the text inside the rectangle
                temperatureLabel.text = value.toFixed(1) + "°C"
            }
        }
    }
}
