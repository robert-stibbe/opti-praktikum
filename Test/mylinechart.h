#ifndef MYLINECHART_H
#define MYLINECHART_H

#include <QLineSeries>
#include <QObject>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class MyLineChart : public QChartView
{
    Q_OBJECT

signals:
 void abstandGeaendert(float abstand);
 void linienlaengeGeaendert(float abstand);

public:
    MyLineChart();
    MyLineChart(QWidget *parent = Q_NULLPTR);

    void mousePressEvent(QMouseEvent *event);

    QPointF pointL;
    QPointF pointR;
    QPointF pointP;

    QLineSeries *lin1;

};

#endif // MYLINECHART_H
