#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QTime>
#include <QThread>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QVBoxLayout> // Add this include directive
#include <QMessageBox>
#include <QMutex>
#include <QQuickWidget>

#include "customchartview.h"

MainWindow::MainWindow(QReadWriteLock *fileMutex, QString filePath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lock(fileMutex)
    , sensorDataFilePath(filePath)
{
    try {
        ui->setupUi(this);
        liveTempSeries = new QLineSeries(this);
        liveHumiditySeries = new QLineSeries(this);
        connect(ui->actionDaily_statistic, &QAction::triggered, this, &MainWindow::showDailyStatisticGraph);
        connect(ui->actionMonthly_statistic, &QAction::triggered, this, &MainWindow::showMonthlyStatisticGraph);
        connect(ui->actionLive_Statistic, &QAction::triggered, this, &MainWindow::showLiveStatisticGraph);
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", QString("An error occurred in MainWindow constructor: %1").arg(e.what()));
    }

}

MainWindow::~MainWindow()
{
    delete liveHumiditySeries;
    delete liveTempSeries;
    delete ui;
}


QMap<QString, QList<QVariantMap>> MainWindow::loadJsonDataFromFile(const QString &fileName)
{
    QMap<QString, QList<QVariantMap>> dataMap;

    QFile file(fileName);
    QReadLocker locker(lock);
    try {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open file" << fileName;
            return dataMap;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (!jsonDoc.isObject()) {
            qWarning() << "JSON is not an object" << fileName;
            return dataMap;
        }

        QJsonObject jsonObject = jsonDoc.object();

        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
            QString dateKey = it.key();
            QJsonArray readingsArray = it.value().toArray();
            QList<QVariantMap> dataList;
            for (const QJsonValue &readingValue : readingsArray) {
                QVariantMap map = readingValue.toObject().toVariantMap();
                dataList.append(map);
            }
            dataMap.insert(dateKey, dataList);
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Error", QString("An error occurred in loadJsonDataFromFile: %1").arg(e.what()));
    }

    return dataMap;
}



QChart* MainWindow::createDailyStatisticChart()
{
    // Load JSON data from file
    // Update the path as needed
    QMap<QString, QList<QVariantMap>> dataMap = loadJsonDataFromFile(sensorDataFilePath);
    qDebug() << "Number of entries in dataMap:" << dataMap.size();
    // Get current date in string format "YYYY-MM-DD"
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");
    qDebug() << "Current date:" << currentDate;

    // Retrieve data for the current date
    QList<QVariantMap> currentData = dataMap.value(currentDate);
    qDebug() << "Number of entries in currentData:" << currentData.size();
    // Create line series for temperature and humidity
    QLineSeries *tempSeries = new QLineSeries();
    QLineSeries *humiditySeries = new QLineSeries();

    // Populate series with data
    for (const auto &entry : currentData) {
        QString timeStr = entry["time"].toString();
        QTime time = QTime::fromString(timeStr, "hh:mm:ss");

        double temp = entry["temp_C"].toDouble();
        double humidity = entry["humidity"].toDouble();

        QDateTime dateTime = QDateTime(QDate::currentDate(), time);
        tempSeries->append(dateTime.toMSecsSinceEpoch(), temp);
        humiditySeries->append(dateTime.toMSecsSinceEpoch(), humidity);
    }

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(tempSeries);
    chart->addSeries(humiditySeries);
    chart->setTitle("Temperature and Humidity on " + currentDate);

    // Configure axes
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(25);
    axisX->setFormat("HH:mm");
    axisX->setTitleText("Time (00:00 - 24:00)");
    QDateTime minTime = QDateTime(QDate::currentDate(), QTime(0, 0));
    QDateTime maxTime = QDateTime(QDate::currentDate(), QTime(23, 59));
    axisX->setRange(minTime, maxTime);
    chart->addAxis(axisX, Qt::AlignBottom);
    tempSeries->attachAxis(axisX);
    humiditySeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Value");

    // Determine the minimum temperature value to set the Y axis range
    double minTemp = 0;
    for (const auto &data : tempSeries->points()) {
        if (data.y() < minTemp) {
            minTemp = data.y();
        }
    }
    axisY->setRange(minTemp, 100);
    chart->addAxis(axisY, Qt::AlignLeft);
    tempSeries->attachAxis(axisY);
    humiditySeries->attachAxis(axisY);

    QLegend *legend = chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);

    tempSeries->setName("Temperature");
    tempSeries->setColor("crimson");
    humiditySeries->setName("Humidity");
    humiditySeries->setColor("skyblue");

    return chart;
}


QChart* MainWindow::createMonthlyStatisticChart()
{
    // Load JSON data from file

    QMap<QString, QList<QVariantMap>> dataMap = loadJsonDataFromFile(sensorDataFilePath);

    // Get current month and year
    QDate currentDate = QDate::currentDate();
    QString currentMonthYear = currentDate.toString("yyyy-MM");

    // Aggregate temperature and humidity data for each day of the current month
    QMap<int, QPair<double, double>> dailyData; // Map day of the month to total temperature and humidity values
    QMap<int, int> dataCount; // Map day of the month to the number of data points

    for (int day = 1; day <= currentDate.daysInMonth(); ++day) {
        dailyData[day] = qMakePair(0.0, 0.0);
        dataCount[day] = 0;
    }

    for (auto it = dataMap.constBegin(); it != dataMap.constEnd(); ++it) {
        QString dateStr = it.key();
        if (dateStr.startsWith(currentMonthYear)) {
            int day = QDate::fromString(dateStr, "yyyy-MM-dd").day();
            QList<QVariantMap> dayData = it.value();
            for (const auto &entry : dayData) {
                double temp = entry["temp_C"].toDouble();
                double humidity = entry["humidity"].toDouble();
                dailyData[day].first += temp;
                dailyData[day].second += humidity;
                dataCount[day]++;
            }
        }
    }

    QBarSeries *series = new QBarSeries();
    QBarSet *tempSet = new QBarSet("Temperature");
    QBarSet *humiditySet = new QBarSet("Humidity");
    QStringList categories;
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    for (int day = 1; day <= currentDate.daysInMonth(); ++day) {
        double avgTemp = (dataCount[day] > 0) ? dailyData[day].first / dataCount[day] : 0.0;
        double avgHumidity = (dataCount[day] > 0) ? dailyData[day].second / dataCount[day] : 0.0;
        if (dataCount[day] > 0) {
            categories << QString::number(day);

            *tempSet << avgTemp;
            *humiditySet << avgHumidity;
        }
    }

    // Set colors for the bar sets
    tempSet->setColor("crimson");
    humiditySet->setColor("skyBlue");

    // Append sets to series
    series->append(tempSet);
    series->append(humiditySet);

    // Create the chart and add the series
    QChart *chart = new QChart();
    chart->setTitle("Average Temperature and Humidity for " + currentDate.toString("MMMM yyyy"));
    chart->addSeries(series);
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);

    series->attachAxis(axisX);


    // Set labels and colors
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    chart->legend()->setVisible(false);

    return chart;
}

QChart *MainWindow::createLiveStatisticChart()
{
    // Get current date in string format "YYYY-MM-DD"
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(liveTempSeries);
    chart->addSeries(liveHumiditySeries);
    chart->setTitle("Live Temperature and Humidity on " + currentDate);

    // Configure axes
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(11); // 5 minutes / 30 seconds per tick = 10 ticks + 1 initial tick
    axisX->setFormat("mm:ss");
    axisX->setTitleText("Time (Last 5 Minutes)");

    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime minTime = currentTime.addSecs(-5 * 60); // 5 minutes ago
    QDateTime maxTime = currentTime;
    axisX->setRange(minTime, maxTime);

    chart->addAxis(axisX, Qt::AlignBottom);
    liveTempSeries->attachAxis(axisX);
    liveHumiditySeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Value");

    // Determine the minimum temperature value to set the Y axis range
    double minTemp = 20;
    double maxHumidity = -10;
    for (const auto &data : liveTempSeries->points()) {
        if (data.y() < minTemp) {
            minTemp = data.y() - 10;
        }
        if (data.y()> maxHumidity)maxHumidity = data.y()+10;
    }
    axisY->setRange(minTemp, maxHumidity);
    chart->addAxis(axisY, Qt::AlignLeft);
    liveTempSeries->attachAxis(axisY);
    liveHumiditySeries->attachAxis(axisY);

    QLegend *legend = chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);

    liveTempSeries->setName("Temperature");
    liveTempSeries->setColor("crimson");
    liveHumiditySeries->setName("Humidity");
    liveHumiditySeries->setColor("skyblue");

    return chart;
}

void MainWindow::showDailyStatisticGraph() {
    QChart *chart = createDailyStatisticChart();
    showChartInNewWindow(chart, "Daily Statistic Graph", true);
}

void MainWindow::showMonthlyStatisticGraph()
{
    QChart *chart = createMonthlyStatisticChart();
    showChartInNewWindow(chart, "Monthly Statistic Graph", false);
}

void MainWindow::showLiveStatisticGraph() {
    QChart *chart = createLiveStatisticChart();
    showChartInNewWindow(chart, "Live Statistic Graph", false);
}

void MainWindow::adjustTickCount(QMainWindow* graphWindow, QChart* chart) {
    // Adjust tick count based on window width
    QDateTimeAxis *axisX = qobject_cast<QDateTimeAxis *>(chart->axes(Qt::Horizontal).at(0));
    if (axisX) {
        int windowWidth = graphWindow->width();

        int tickCount = (windowWidth < 850) ? 15 : 25; // Adjust tick count based on window width
        if(windowWidth<500) tickCount = 5;
        axisX->setTickCount(tickCount);


        // Adjust label format based on window width
        QString labelFormat = (windowWidth < 1100) ? "h" : "HH:mm";
        axisX->setFormat(labelFormat);
    }
}

void MainWindow::updateTemperature(float celsius, float fahrenheit)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime minTime = currentTime.addSecs(-5 * 60); // 5 minutes ago
    if (liveTempSeries) {
        QDateTime time = QDateTime::currentDateTime();
        liveTempSeries->append(time.toMSecsSinceEpoch(), celsius);
        while (!liveTempSeries->points().isEmpty() && liveTempSeries->points().first().x() < minTime.toMSecsSinceEpoch()) {
            liveTempSeries->remove(0);
        }
    }
}

void MainWindow::updateHumidity(float humidity)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime minTime = currentTime.addSecs(-5 * 60); // 5 minutes ago
    if (liveHumiditySeries) {
        QDateTime time = QDateTime::currentDateTime();
        liveHumiditySeries->append(time.toMSecsSinceEpoch(), humidity);
        while (!liveHumiditySeries->points().isEmpty() && liveHumiditySeries->points().first().x() < minTime.toMSecsSinceEpoch()) {
            liveHumiditySeries->remove(0);
        }
    }
}

void MainWindow::showChartInNewWindow(QChart *chart, const QString &title,  bool adjustable)
{
    QMainWindow *graphWindow = new QMainWindow;
    QWidget *graphWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    QChartView *chartView = new CustomChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);
    graphWidget->setLayout(layout);

    graphWindow->setCentralWidget(graphWidget);
    graphWindow->setWindowTitle(title);
    graphWindow->resize(800, 600);

    if(adjustable)
    connect(dynamic_cast<CustomChartView*>(chartView), &CustomChartView::resized, this, [graphWindow, chart, this]() {
        adjustTickCount(graphWindow, chart);
    });
    graphWindow->show();
}
