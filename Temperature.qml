import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {

    property real temperature: 0
    property int heightProperty: 600

    Text {
        id: tempTitle
        text: "Temperature"
        font.pixelSize: 18
        anchors.horizontalCenter: parent.horizontalCenter // Center horizontally
        anchors.top: parent.top // Align to the top
        anchors.bottomMargin: heightProperty/8
    }

    Rectangle {
        width: heightProperty / 5
        height: heightProperty / 5
        anchors.horizontalCenter: parent.horizontalCenter // Center horizontally
        // Center vertically by shifting up by half of the rectangle's height
        anchors.top: tempTitle.bottom
        anchors.topMargin: 10 // Adjust as needed

        // Create a thicker border to simulate the Dial
        border.color: "crimson"
        border.width: 10
        radius: width / 2

        Text {
            id: temperatureLabel
            font.bold: true
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
    }


    function updateTemperature(newTemperature) {
        temperature = newTemperature
        temperatureLabel.text = temperature.toFixed(1) + "°C";
    }

    function updateCircle(newHeight){
        heightProperty = newHeight
    }
}
