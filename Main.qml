import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Temperature and Humidity Display"

    RowLayout {
        anchors.centerIn: parent
        spacing: 50

        Temperature{
        id: temperatureObj
        }

        Humidity{
        id:humidityObj
        }
    }
}
