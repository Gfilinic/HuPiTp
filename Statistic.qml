import QtQuick
import QtGraphs

Item {

    property var dataPoints: [
        { x: 0, y: 1 },
        { x: 1, y: 3 },
        { x: 2, y: 4 },
        { x: 3, y: 6 },
        { x: 4, y: 8 }
        // Add more data points as needed
    ]

    GraphsView {
        id: chart
        anchors.fill: parent

        LineSeries {
            id: temperatureSeries
            name: "Line 2"
            XYPoint { x: 0; y: 3 }
            XYPoint { x: 1; y: 1 }
            XYPoint { x: 2; y: 2 }
            XYPoint { x: 3; y: 5 }
            XYPoint { x: 4; y: 7 }
            color: "crimson"
            axisX: ValueAxis {
                min: 0
                max: 23


            }
            axisY: ValueAxis {
                min: -20
                max: 100

            }
        }

        LineSeries {
            id: humiditySeries
            name: "Line 2"
            XYPoint { x: 0; y: 6 }
            XYPoint { x: 1; y: 8 }
            XYPoint { x: 2; y: 1 }
            XYPoint { x: 3; y: 6 }
            XYPoint { x: 4; y: 2 }
            color: "skyblue"
            axisX: ValueAxis {
                min: 0
                max: 23


            }
            axisY: ValueAxis {
                min: -20
                max: 100


            }
        }

    }


}
