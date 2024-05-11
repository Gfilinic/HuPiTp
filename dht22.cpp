#include "dht22.h"
#include <stdio.h>
#include <time.h>
#include <wiringPi.h>

DHT22::DHT22(QObject *parent) : QObject(parent)
{
    // Constructor implementation
}

short DHT22::readData()
{
    unsigned short val = 0x00;
    unsigned short signal_length = 0;
    unsigned short val_counter = 0;
    unsigned short loop_counter = 0;

    while (1)
    {
        // Count only HIGH signal
        while (digitalRead(signal) == HIGH)
        {
            signal_length++;

            // When sending data ends, high signal occur infinite.
            // So we have to end this infinite loop.
            if (signal_length >= 200)
            {
                return -1;
            }

            delayMicroseconds(1);
        }

        // If signal is HIGH
        if (signal_length > 0)
        {
            loop_counter++;	// HIGH signal counting

            // The DHT22 sends a lot of unstable signals.
            // So extended the counting range.
            if (signal_length < 10)
            {
                // Unstable signal
                val <<= 1;		// 0 bit. Just shift left
            }

            else if (signal_length < 30)
            {
                // 26~28us means 0 bit
                val <<= 1;		// 0 bit. Just shift left
            }

            else if (signal_length < 85)
            {
                // 70us means 1 bit
                // Shift left and input 0x01 using OR operator
                val <<= 1;
                val |= 1;
            }

            else
            {
                // Unstable signal
                return -1;
            }

            signal_length = 0;	// Initialize signal length for next signal
            val_counter++;		// Count for 8 bit data
        }

        // The first and second signal is DHT22's start signal.
        // So ignore these data.
        if (loop_counter < 3)
        {
            val = 0x00;
            val_counter = 0;
        }

        // If 8 bit data input complete
        if (val_counter >= 8)
        {
            // 8 bit data input to the data array
            data[(loop_counter / 8) - 1] = val;

            val = 0x00;
            val_counter = 0;
        }
    }
}

void DHT22::readAndOutputSensorDataAsJson(const QString &filename) // Modified to accept filename
{
    float humidity;
    float celsius;
    float fahrenheit;
    short checksum;
    short maxtry;
    struct timespec start, end;
    double elapsedTime;
    QFile file(filename); // Create QFile object with the given filename

    // GPIO Initialization
    if (wiringPiSetupGpio() == -1)
    {
        fprintf(stderr, "ERROR: GPIO Initialization FAILED.\n");
        throw std::runtime_error("GPIO Initialization FAILED");
    }

    maxtry = 20;
    //start timer
    clock_gettime(CLOCK_REALTIME, &start);
    int i = 0;
    while(true){

        pinMode(signal, OUTPUT);

        // Send out start signal
        digitalWrite(signal, LOW);
        delay(20);           // Stay LOW for 5~30 milliseconds
        pinMode(signal, INPUT);        // 'INPUT' equals 'HIGH' level. And signal read mode

        readData();         // Read DHT22 signal

        // The sum is maybe over 8 bit like this: '0001 0101 1010'.
        // Remove the '9 bit' data using AND operator.
        checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;

        // If Check-sum data is correct (NOT 0x00), display humidity and temperature
        if (data[4] == checksum && checksum != 0x00)
        {
            // stop timer
            clock_gettime(CLOCK_REALTIME, &end);
            // compute and print the elapsed time in sec
            elapsedTime = (end.tv_sec - start.tv_sec);

            // * 256 is the same thing '<< 8' (shift).
            humidity = ((data[0] * 256) + data[1]) / 10.0;
            celsius = (((data[2] & 0x7F)*256) + data[3]) / 10.0; //changed acc. Juergen Wolf-Hofer

            // If 'data[2]' data like 1000 0000, It means minus temperature
            if (data[2] == 0x80)
            {
                celsius *= -1;
            }

            fahrenheit = ((celsius * 9) / 5) + 32;

            // time
            QDateTime dateTime = QDateTime::currentDateTime();

            // Create JSON object
            QJsonObject json;
            json["elapsed_time_sec"] = elapsedTime;
            json["try_count"] = i;
            json["datetime"] = dateTime.toString(Qt::ISODate);
            json["temp_C"] = celsius;
            json["temp_F"] = fahrenheit;
            json["humid_perc"] = humidity;

            // Write JSON object to file
            if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                QJsonDocument doc(json);
                file.write(doc.toJson());
                file.write("\n"); // Add newline for readability
                file.close();
            } else {
                fprintf(stderr, "Error opening file for writing.\n");
            }

            emit temperatureUpdated(celsius, fahrenheit);
            emit humidityUpdated(humidity);
        } else {
            fprintf(stderr, "Error: Invalid data (attempt %d). Try again...\n", i+1);
        }

        // Initialize data array for next loop
        for (unsigned char j = 0; j < 5; j++)
        {
            data[j] = 0;
        }

        delay(2000);    // DHT22 average sensing period is 2 seconds
        i++;
    }

    fprintf(stderr, "Maximum attempts reached. Stop reading.\n");
    throw std::runtime_error("Maximum attempts reached");
}

