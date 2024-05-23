#include <QApplication>
#include <QQuickWidget>
#include <QThread>
#include <QQmlApplicationEngine>
#include "mainwindow.h"
#include "dht22.h"
#include <QMutex>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    try {
        QMutex fileMutex;

        // MainWindow setup
        MainWindow w(&fileMutex);

        // QQuickWidget setup for QML
        QQuickWidget *qmlWidget = new QQuickWidget;
        qmlWidget->setSource(QUrl(QStringLiteral("qrc:/HuPiTp/Main.qml")));
        qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        w.setCentralWidget(qmlWidget);

        // DHT22 setup
        DHT22 dht22(&fileMutex, &w); // Create DHT22 object on the stack
        QThread sensorThread(&w);     // Create QThread object on the stack
        dht22.moveToThread(&sensorThread);
        QString fileName = "sensorDataDHT22.json";

        QObject::connect(&sensorThread, &QThread::started, &dht22, [fileName, &dht22]() {
            try {
                dht22.readAndOutputSensorDataAsJson(fileName);
            } catch (const std::exception &e) {
                qCritical() << "Exception in readAndOutputSensorDataAsJson:" << e.what();
            }
        });
        QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
            sensorThread.quit();
            sensorThread.wait();
        });
        sensorThread.start();

        // Connect signals to update both QWidget and QML interfaces
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
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", QString("An error occurred in main.cpp: %1").arg(e.what()));
        return -1;
    }
}
