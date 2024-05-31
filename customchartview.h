#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QChartView>

class CustomChartView : public QChartView
{
    Q_OBJECT
public:
    explicit CustomChartView(QChart *chart, QWidget *parent = nullptr);
signals:
    void resized();

protected:
    void resizeEvent(QResizeEvent *event) override;
};


#endif // CUSTOMCHARTVIEW_H
