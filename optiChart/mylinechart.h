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

     enum reduceAlgo {Unveraendert, DouglasPeucker, Lang, Ralph, DpRalph, DpLang };

    void switchChartType();
    void initMyChart();

    void initBasisWerte();

    void setSchrittweite(double sw);
    void stopDenBloedenTimer();
    switchTimer();
    void swichReduceType(reduceAlgo rt);
    void createMarker();

private:
    QPointF pointL;
    QPointF pointR;
    QPointF pointP;
    reduceAlgo reduceType ;
    QList<QPointF> oriWerte;
    QLineSeries *lin1;
    QScatterSeries *series1;
    QLineSeries *druckWerte ;
    float schrittweite1;
    QTimer *timer;
    bool mausklickAktiv ;
    bool timerAn;

public slots:
    void update();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

};

#endif // MYLINECHART_H
