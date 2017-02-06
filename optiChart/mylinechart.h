#ifndef MYLINECHART_H
#define MYLINECHART_H

#include <QLineSeries>
#include <QObject>
#include <QScatterSeries>
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

    void switchChartType();
    initMyChart();

    void initBasisWerte();

    void setSchrittweite(double sw);
    void stopDenBloedenTimer();
private:
    QPointF pointL;
    QPointF pointR;
    QPointF pointP;

    QList<QPointF> oriWerte;
    QLineSeries *lin1;
    QScatterSeries *series1;
    QLineSeries *druckWerte2 ;
    float schrittweite1;
     QTimer *timer;

public slots:
    void update();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

};

#endif // MYLINECHART_H
