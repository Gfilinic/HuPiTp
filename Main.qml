import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Temperature and Humidity Display"
    property real temperatureValue: 0
    property real temperatureFValue: 0
    property real humidityValue: 0
    property bool tChange: false
    property bool hChange: false



        Temperature{
            id: temperatureObj
            temperature: temperatureValue // Pass temperatureValue to Temperature
            anchors.left:parent.left
        }

        Humidity{
            id:humidityObj
            humidity: humidityValue
            anchors.right: parent.right
        }




    Log {
        id: log
        anchors {

            bottom: parent.bottom
            left: parent.left
            right: parent.right
            topMargin: 50
        }
        width:parent.width

    }
    Component.onCompleted: {
        temperatureValue = 50
        humidityValue = 40
    }
    onTemperatureValueChanged: {
        temperatureObj.updateTemperature(temperatureValue)
        tChange = true
        checkUpdateLog()

    }

    onHumidityValueChanged:{
        humidityObj.updateHumidity(humidityValue)
        hChange = true
        checkUpdateLog()
    }

    function checkUpdateLog(){
        if (tChange && hChange){
            tChange = false
            hChange = false
            log.updateLog(temperatureValue,temperatureFValue,humidityValue)
        }
    }
}

