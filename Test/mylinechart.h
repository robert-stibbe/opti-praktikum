#ifndef MYLINECHART_H
#define MYLINECHART_H

#include <QObject>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class MyLineChart : public QChartView
{
public:
    MyLineChart();
    MyLineChart(QWidget *parent = Q_NULLPTR);

    void mousePressEvent(QMouseEvent *event);

    QPointF pointL;
    QPointF pointR;
};

#endif // MYLINECHART_H
