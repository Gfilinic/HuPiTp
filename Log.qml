import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: root
    color: "white"
    border.color: "black"
    border.width: 3
    width:parent.width
    height:parent.height/1.5

    property string log: ""
    ScrollView {

            Text{
                padding: 10
                text: log
                font.pixelSize: 16
            }
        }



        function updateLog(temperature,temperatureF, humidity){
            log += "\nTemperature: " + temperature + "°C (" + temperatureF + "°F)\n";
            log += "Humidity: " + humidity + "%\n";
            log += "----------------------------------------------\n"
        }

}
