#include "polarchart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const qreal angularMin = 0;
    const qreal angularMax = 360;

    const qreal radialMin = 0;
    const qreal radialMax = 100;



/*    for (int winkel = angularMin; winkel <= angularMax; winkel += 5)
    {
       // int radius =  (winkel / radialMax) * radialMax;
        int radius = (1 + sin(winkel / 360.0 * 2*3.141592653589793238 ) ) / 2 * (radialMax - radialMin) +radialMin;
        series2->append(winkel, radius);
    }
*/
    QPolarChart *chart = new QPolarChart();

   // chart->addSeries(druckWerte);
    chart->setTitle("Pfeiltasten rechts/links = drehen, +/- zum zoomen ,Lehrtaste zum wechseln.");

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(10); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(30);
    radialAxis->setLabelFormat("%d");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);



    radialAxis->setRange(radialMin, radialMax);
    angularAxis->setRange(angularMin, angularMax);

    PolarChart *chartView = new PolarChart();
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->initBasisWerte(angularAxis,radialAxis);


    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(800, 600);
    window.show();

    return a.exec();
}
