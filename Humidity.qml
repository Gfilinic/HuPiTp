import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    property real humidity: 0
    property int heightProperty: 600

    Text {
        id: humidityTitle
        text: "Humidity"
        font.pixelSize: 18
        anchors.horizontalCenter: parent.horizontalCenter // Center horizontally
        anchors.top: parent.top // Align to the top
        anchors.bottomMargin: heightProperty/8 // Adjust as needed
    }

    Rectangle {
        width: heightProperty / 5
        height: heightProperty / 5
        anchors.horizontalCenter: parent.horizontalCenter // Center horizontally
        // Center vertically by shifting up by half of the rectangle's height
        anchors.top: humidityTitle.bottom
        anchors.topMargin: 10 // Adjust as needed

        // Create a thicker border to simulate the Dial
        border.color: "skyblue"
        border.width: 10
        radius: width / 2

        Text {
            id: humidityLabel
            font.pixelSize: 16
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
    }

    function updateHumidity(newHumidity) {
        humidity = newHumidity;
        humidityLabel.text = humidity.toFixed(1) + " %";
    }

    function updateCircle(newHeight){
        heightProperty = newHeight
    }
}
