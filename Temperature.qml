import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    spacing: 20
    property real temperature: 0
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
            id:temperatureLabel

            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
    }


    function updateTemperature(newTemperature) {
        temperature=newTemperature
        temperatureLabel.text = temperature.toFixed(1) + "°C";
    }
}
