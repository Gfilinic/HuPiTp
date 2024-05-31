#include <QApplication>
#include <QQuickWidget>
#include <QThread>
#include <QQmlApplicationEngine>
#include "mainwindow.h"


#include <QMessageBox>
#include <QReadWriteLock>
#include <QtConcurrent>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    try {
        QReadWriteLock fileMutex;

        // MainWindow setup

        // Get the current directory of the executable
        QString currentDir = QCoreApplication::applicationDirPath();
        qDebug()<< "Current directory: " << currentDir.toStdString();

        // Set the filename
        QString fileName = QDir(currentDir).filePath("../../sensorDataDHT22.json");
        qDebug() << "Full path to the JSON file: " << fileName.toStdString();
        MainWindow w(&fileMutex, fileName);

        // QQuickWidget setup for QML
        QQuickWidget *qmlWidget = new QQuickWidget;
        qmlWidget->setSource(QUrl(QStringLiteral("qrc:/HuPiTp/Main.qml")));
        qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        w.setCentralWidget(qmlWidget);


        w.show();
        QRandomGenerator *generator = QRandomGenerator::global();

        // Run the sensor data mocking in a separate thread
        // Run the sensor data mocking in a separate thread
        QFuture<void> future = QtConcurrent::run([&w, generator]() {
            while (true) {
                // Generate random temperature and humidity values
                float temperature = generator->generateDouble() * (25.0 - 20.0) + 20.0;
                float humidity = generator->generateDouble() * (50.0 - 30.0) + 30.0;

                float tempF = temperature * 9/5 + 32;
                qDebug() << "Temp:" << temperature <<"\nHumidity:" << humidity;
                // Update the temperature and humidity in the MainWindow
                w.updateTemperature(temperature,tempF);
                w.updateHumidity(humidity);

                // Sleep for 2 seconds
                QThread::sleep(2);
            }
        });

        return app.exec();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", QString("An error occurred in main.cpp: %1").arg(e.what()));
        return -1;
    }
}
