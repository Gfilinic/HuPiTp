#include <QApplication>
#include <QQuickWidget>
#include <QThread>
#include <QQmlApplicationEngine>
#include "mainwindow.h"
#include "dht22.h"
#include <QMutex>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMutex fileMutex;
    // MainWindow setup



    // QQuickWidget setup for QML
    QQuickWidget *qmlWidget = new QQuickWidget;
    qmlWidget->setSource(QUrl(QStringLiteral("qrc:/HuPiTp/Main.qml")));
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);


    // DHT22 setup
    DHT22 dht22(&fileMutex);
    QThread sensorThread;
    dht22.moveToThread(&sensorThread);
    QString fileName = "sensorDataDHT22.json";

    QObject::connect(&sensorThread, &QThread::started, &dht22, [fileName, &dht22]() {
        dht22.readAndOutputSensorDataAsJson(fileName);
    });
    sensorThread.start();


    MainWindow w(&fileMutex, &dht22);
    w.setCentralWidget(qmlWidget);


    // Connect signals to updateboth QWidget and QML interfaces
    QObject::connect(&dht22, &DHT22::temperatureUpdated, &w, [&](float celsius, float fahrenheit) {
        w.updateTemperature(celsius, fahrenheit);
        if (qmlWidget->rootObject()) {
            qmlWidget->rootObject()->setProperty("temperatureValue", celsius);
            qmlWidget->rootObject()->setProperty("temperatureFValue", fahrenheit);
        }
    });

    QObject::connect(&dht22, &DHT22::humidityUpdated, &w, [&](float humidity) {
        w.updateHumidity(humidity);
        if (qmlWidget->rootObject()) {
            qmlWidget->rootObject()->setProperty("humidityValue", humidity);
        }
    });
    w.show();
    return app.exec();
}
