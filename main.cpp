#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread> // Include QThread for threading
#include "dht22.h" // Include the header for DHT22 here

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DHT22 dht22;
    QThread sensorThread;
    dht22.moveToThread(&sensorThread);
    QString fileName = "sensorDataDHT22.json";
    QObject::connect(&sensorThread, &QThread::started, &dht22, [fileName, &dht22]() {
        dht22.readAndOutputSensorDataAsJson(fileName); // Pass the file name to the function
    });
    sensorThread.start();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/HuPiTp/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    // Connect signals to update temperature and humidity
    QObject::connect(&dht22, &DHT22::temperatureUpdated, &engine, [&](float celsius, float fahrenheit) {
        engine.rootObjects().first()->setProperty("temperatureValue", celsius);
        engine.rootObjects().first()->setProperty("temperatureFValue", fahrenheit);
    });

    QObject::connect(&dht22, &DHT22::humidityUpdated, &engine, [&](float humidity) {
        engine.rootObjects().first()->setProperty("humidityValue", humidity);
    });

    return app.exec();
}
