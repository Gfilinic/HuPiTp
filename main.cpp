#include <QApplication>
#include <QQuickWidget>
#include <QThread>
#include <QQmlApplicationEngine>
#include "mainwindow.h"
#include "dht22.h"
#include <QMutex>
#include <QMessageBox>
#include <QReadWriteLock>
#include <QtConcurrent>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    try {
        QReadWriteLock fileMutex;

        // MainWindow setup

        QString fileName = "sensorDataDHT22.json";
        MainWindow w(&fileMutex, fileName);

        // QQuickWidget setup for QML
        QQuickWidget *qmlWidget = new QQuickWidget;
        qmlWidget->setSource(QUrl(QStringLiteral("qrc:/HuPiTp/Main.qml")));
        qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        w.setCentralWidget(qmlWidget);

        // DHT22 setup
        DHT22 *dht22 = new DHT22(&fileMutex); // Create DHT22 object on the stack


        auto readDataFunc = [=]() {
            dht22->readAndOutputSensorDataAsJson(fileName);
        };

        // Use QtConcurrent::run with the lambda function
        QFuture<void> future = QtConcurrent::run(readDataFunc);

        // Connect signals to update both QWidget and QML interfaces
        QObject::connect(dht22, &DHT22::temperatureUpdated, &w, [&](float celsius, float fahrenheit) {
            w.updateTemperature(celsius, fahrenheit);
            if (qmlWidget->rootObject()) {
                qmlWidget->rootObject()->setProperty("temperatureValue", celsius);
                qmlWidget->rootObject()->setProperty("temperatureFValue", fahrenheit);
            }
        });

        QObject::connect(dht22, &DHT22::humidityUpdated, &w, [&](float humidity) {
            w.updateHumidity(humidity);
            if (qmlWidget->rootObject()) {
                qmlWidget->rootObject()->setProperty("humidityValue", humidity);
            }
        });

        w.show();

        return app.exec();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", QString("An error occurred in main.cpp: %1").arg(e.what()));
        return -1;
    }
}
