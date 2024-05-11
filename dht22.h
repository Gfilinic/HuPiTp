#ifndef DHT22_H
#define DHT22_H

#include <QObject>
#include <QJsonObject> // Include the necessary header
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <QFile>

class DHT22 : public QObject
{
    Q_OBJECT
public:
    explicit DHT22(QObject *parent = nullptr);

signals:
    void temperatureUpdated(float celsius, float fahrenheit);
    void humidityUpdated(float humidity);

public slots:
    void readAndOutputSensorDataAsJson(const QString &filename);

private:
    static const unsigned short signal = 18;
    unsigned short data[5] = {0, 0, 0, 0, 0};

    short readData();
};


#endif // DHT22_H
