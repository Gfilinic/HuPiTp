import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 600
    title: "Temperature and Humidity Display"
    property real temperatureValue: 0
    property real temperatureFValue: 0
    property real humidityValue: 0

    // Menu bar with only the "Statistics" option
        menuBar: MenuBar {
            Menu {
                title: qsTr("&View")
                Action {
                    text: qsTr("&Statistics")
                    onTriggered: statisticsWindow.visible = !statisticsWindow.visible
                }
            }
        }

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
        width:parent.width
    }

    Component.onCompleted: {
        temperatureValue = 50
        humidityValue = 44
        temperatureValue = 51
        humidityValue = 42
        temperatureValue = 52
        humidityValue = 43
        temperatureValue = 50
        humidityValue = 44
        temperatureValue = 51
        humidityValue = 42
        temperatureValue = 52
        humidityValue = 43
        temperatureValue = 50
        humidityValue = 44
        temperatureValue = 51
        humidityValue = 42
        temperatureValue = 52
        humidityValue = 43
    }

    onTemperatureValueChanged: {
        temperatureObj.updateTemperature(temperatureValue)
        log.updateTemperatureLog(temperatureValue,temperatureFValue)
    }

    onHumidityValueChanged:{
        humidityObj.updateHumidity(humidityValue)
        log.updateHumidityLog(humidityValue)
    }

    // Statistics window
    Window {
        id: statisticsWindow
        visible: false
        width: 600
        height: 400
        title: "Statistics"

        // Load the Statistic.qml file here
        Statistic {
            anchors.fill: parent
        }
    }
}
