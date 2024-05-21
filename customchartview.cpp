#include "customchartview.h"
CustomChartView::CustomChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
    // Additional initialization code if needed
}
void CustomChartView::resizeEvent(QResizeEvent *event)
{
    QChartView::resizeEvent(event);
    emit resized();
}
