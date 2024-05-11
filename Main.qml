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
    RowLayout {
        anchors.centerIn: parent
        spacing: 50

        Temperature{
        id: temperatureObj
        temperature: temperatureValue // Pass temperatureValue to Temperature
        temperatureF: temperatureFValue // Pass temperatureFValue to Temperature
        }

        Humidity{
        id:humidityObj
        humidity: humidityValue
        }
    }
}
