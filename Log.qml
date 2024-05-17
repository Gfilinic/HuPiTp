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
    height:parent.height / 1.7
    anchors {
        bottomMargin: 1/50 * parent.height
        leftMargin: 1/50 * parent.width
        rightMargin: 1/50 * parent.width
    }
    property string log: ""
    ScrollView {
        id: scrollView
        width: parent.width
        height: parent.height
        contentWidth: text.width
        contentHeight: text.height

        Text{
            id: text
            padding: 10
            text: log
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            width: scrollView.width
            height: contentHeight
        }
    }

    function updateTemperatureLog(temperature, temperatureF) {
        temperature = temperature.toFixed(2);
        temperatureF = temperatureF.toFixed(2);
        var newEntry = "----------------------------------------------\n";
        newEntry += "\nTemperature: " + temperature + "°C (" + temperatureF + "°F)\n";
        log = newEntry + log;
    }
    function updateHumidityLog(humidity) {
        humidity = humidity.toFixed(2);
        var newEntry = "Humidity: " + humidity + "%\n";


        log = newEntry + log;
    }

}
