import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Item {
    visible: true
    anchors.fill: parent
    property real temperatureValue: 0
    property real temperatureFValue: 0
    property real humidityValue: 0

    onHeightChanged: {
        humidityObj.updateCircle(height)
        temperatureObj.updateCircle(height)
    }

    Temperature {
        id: temperatureObj
        temperature: temperatureValue // Pass temperatureValue to Temperature
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 1/5 * parent.width // Add space from the left
        anchors.topMargin: 1/20 * parent.height  // Add space from the top
    }

    Humidity {
        id: humidityObj
        humidity: humidityValue
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 1/5 * parent.width   // Add space from the right
        anchors.topMargin: 1/20 * parent.height// Add space from the top
    }

    Log {
        id: log
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            topMargin: 50
        }
        width: parent.width
    }

    Component.onCompleted: {

    }

    onTemperatureValueChanged: {
        temperatureObj.updateTemperature(temperatureValue)
        log.updateTemperatureLog(temperatureValue,temperatureFValue)
    }

    onHumidityValueChanged:{
        humidityObj.updateHumidity(humidityValue)
        log.updateHumidityLog(humidityValue)
    }
}

