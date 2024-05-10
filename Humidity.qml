import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    ColumnLayout {
        spacing: 20

        Text {
            text: "Humidity"
            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            width: 120
            height: 120

            // Create a thicker border to simulate the Dial
            border.color: "skyblue"
            border.width: 10
            radius: width / 2

            Text {
                text: humidityDial.value.toFixed(1) + "°C"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }
        }

        Dial {
            id: humidityDial
            from: 0
            visible: false
            to: 100
            value: 30
            stepSize: 0.01
            enabled: false // Making the dial read-only
            width: 0
            height: 0 // Hide the dial, we're using the Rectangle above as the visual representation

            onValueChanged: {
                // Update the text inside the rectangle
                humidityLabel.text = value.toFixed(1) + "°C"
            }
        }
    }
}




