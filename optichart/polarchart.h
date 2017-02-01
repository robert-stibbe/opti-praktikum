#ifndef POLARCHART_H
#define POLARCHART_H
#include <QtCharts/QChartView>
#include <QtCharts/QPolarChart>
#include <QLineSeries>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE

class PolarChart : public QChartView
{
    Q_OBJECT

public:
    PolarChart(QWidget *parent = 0);
    void initBasisWerte(QtCharts::QValueAxis *angularAxis, QtCharts::QValueAxis *radialAxis);

private slots:
    void update();


protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void switchChartType();
//   QList<QAbstractSeries *> oriSeriesList ;
    QList<QPointF> oriWerte;
    QLineSeries *druckWerte2 ;
};

#endif // POLARCHART_H
