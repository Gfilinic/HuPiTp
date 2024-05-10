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

        ColumnLayout {
            spacing: 20

            Text {
                text: "Temperature"
                font.pixelSize: 18
                Layout.alignment: Qt.AlignHCenter
            }

            Dial {
                id: temperatureDial
                from: -20
                to: 40
                value: 25
                stepSize: 0.01
                enabled: false // Making the dial read-only
                width: 175
                height: 175

                onValueChanged: {
                    temperatureLabel.text = value.toFixed(1) + "°C"
                }
            }

            Text {
                id: temperatureLabel
                text: temperatureDial.value.toFixed(1) + "°C"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter // Aligning text to the center
                Layout.alignment: Qt.AlignHCenter
            }
        }

        ColumnLayout {
            spacing: 20

            Text {
                text: "Humidity"
                font.pixelSize: 18
                horizontalAlignment: Text.AlignHCenter // Aligning text to the center
                Layout.alignment: Qt.AlignHCenter
            }

            Dial {
                id: humidityDial
                from: 0
                to: 100
                value: 30
                stepSize: 0.01
                enabled: false // Making the dial read-only
                width: 175
                height: 175

                onValueChanged: {
                    humidityLabel.text = value.toFixed(0) + "%"
                }
            }

            Text {
                id: humidityLabel
                text: humidityDial.value.toFixed(0) + "%"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter // Aligning text to the center
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
