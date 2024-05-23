#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QQuickWidget>
#include <QQuickItem>
#include <QQmlContext>
#include <QQmlEngine>
#include <QReadWriteLock>
#include"dht22.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QReadWriteLock *fileMutex, QString filePath, QWidget *parent = nullptr);
    ~MainWindow();
    void updateTemperature(float celsius, float fahrenheit);
    void updateHumidity(float humidity);

public slots:
    void showDailyStatisticGraph();
    void showMonthlyStatisticGraph();
    void showLiveStatisticGraph();
private slots:
    void adjustTickCount(QMainWindow* graphWindow, QChart* chart);
private:
    Ui::MainWindow *ui;

    QMap<QString, QList<QVariantMap>> loadJsonDataFromFile(const QString& fileName);
    QChart* createDailyStatisticChart();
    QChart* createMonthlyStatisticChart();
    QChart* createLiveStatisticChart();

    QLineSeries *liveTempSeries; // Added
    QLineSeries *liveHumiditySeries; // Added

    QQuickWidget *mainQmlWidget;
    DHT22 *dht22;
    QString sensorDataFilePath;
    QReadWriteLock *lock;

    void showChartInNewWindow(QChart *chart, const QString &title, bool adjustTickCount);

};

#endif // MAINWINDOW_H
