import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


    ColumnLayout {
        spacing: 20
        property real humidity: 0
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
                id:humidityLabel

                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }
        }


    function updateHumidity(newHumidity) {
            humidity = newHumidity;
            humidityLabel.text = humidity.toFixed(1) + " %";
        }
}




