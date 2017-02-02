#include "mainwindow.h"
#include "mylinechart.h"
#include "polarchart.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include <QPolarChart>
#include <QScatterSeries>
#include <QValueAxis>


QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Verbinde Signal und Slot
    QObject::connect(  ui->graphicsView, &MyLineChart::abstandGeaendert,
                       this,             &setAbstand );

    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(10, 10);

    QScatterSeries *series1 = new QScatterSeries();
    series1->append(5,5);
    series1->append(6,6);

     QChart *chart = new QChart();
     chart->legend()->hide();
     chart->addSeries(series1);

     QValueAxis *xAxis = new QValueAxis();
     xAxis->setTickCount(10);

     xAxis->setLabelFormat(("%.1f ")); //%.0f \u00B0
     xAxis->setShadesVisible(true);
     xAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
     chart->addAxis(xAxis, Qt::AlignLeft);

     QValueAxis *yAxis = new QValueAxis();
     yAxis->setTickCount(10);
     yAxis->setLabelFormat("%d ");
     chart->addAxis(yAxis, Qt::AlignBottom);
     yAxis->setRange(0, 10);
     xAxis->setRange(0, 10);


   //  chartView->setRenderHint(QPainter::Antialiasing);
  //   chart->createDefaultAxes();
     chart->setTitle("Klicke auf den Chart : linke und rechte Maustaste = linie1 backbutton = punkt ");

//     MyLineChart *chartView = new MyLineChart();
  //   chartView->setChart(chart);

    ui->graphicsView->setChart(chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setAbstand(float abstand)
{
    QString a = QString::number(abstand);

     ui->abstand->setText(a);
}

void MainWindow::on_pushButton_4_clicked()
{

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
    chart->setTitle("Leertaste zum Wechseln.");

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(10); // First and last ticks are co-located on 0/360 angle.

    angularAxis->setLabelFormat(("%.1f Grad")); //%.0f \u00B0
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(20);
    radialAxis->setLabelFormat("%d bar");
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

}

