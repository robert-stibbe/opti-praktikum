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


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTestdaten(Dtyp datentyp)
{

    const qreal angularMin = 0;
    const qreal angularMax = 360;

    const qreal radialMin = 0;
    const qreal radialMax = 100;



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

    QList<QPointF> oriWerte;
    if (datentyp == VIERTAKT)
    {
        oriWerte.append( QPointF (0,10));
        oriWerte.append( QPointF (45, 7));
        oriWerte.append( QPointF (90, 5));
        oriWerte.append( QPointF (135, 6));
        oriWerte.append( QPointF (180, 8));
        oriWerte.append( QPointF (210, 14));
        oriWerte.append( QPointF (240, 22));
        oriWerte.append( QPointF (270, 30));
        oriWerte.append( QPointF (300, 40));
        oriWerte.append( QPointF (320, 50));
        oriWerte.append( QPointF (350, 70));
        oriWerte.append( QPointF (360, 100));
        oriWerte.append( QPointF (0, 100));
        oriWerte.append( QPointF (5, 90));
        oriWerte.append( QPointF (10, 80));
        oriWerte.append( QPointF (20, 60));
        oriWerte.append( QPointF (50, 45));
        oriWerte.append( QPointF (60, 40));
        oriWerte.append( QPointF (90, 35));
        oriWerte.append( QPointF (110, 30));
        oriWerte.append( QPointF (130, 26));
        oriWerte.append( QPointF (150, 23));
        oriWerte.append( QPointF (180, 20));
        oriWerte.append( QPointF (210, 20));
        oriWerte.append( QPointF (240, 20));
        oriWerte.append( QPointF (270, 20));
        oriWerte.append( QPointF (290, 18));
        oriWerte.append( QPointF (310, 15));
        oriWerte.append( QPointF (340, 12));
        oriWerte.append( QPointF (360, 10));
    }
    if (datentyp == LINEAR)
    {
        for (int winkel = angularMin; winkel <= angularMax; winkel += 5)
           {
              // int radius =  (winkel / radialMax) * radialMax;
              // int radius = (1 + sin(winkel / 360.0 * 2*3.141592653589793238 ) ) / 2 * (radialMax - radialMin) +radialMin;
              int radius = winkel;
              oriWerte.append(QPointF (winkel, radius));
           }
    }
    QLineSeries *druckWerte = new  QLineSeries();
    druckWerte->append(oriWerte);
    chart->addSeries(druckWerte);

/*
  //  PolarChart *chartView = new PolarChart();
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->initBasisWerte(angularAxis,radialAxis);
*/
   ui->graphicsView->setChart(chart);

}

void MainWindow::showPunktevermessung()
{
// Verbinde Signal und Slot
QObject::connect(  ui->graphicsView, &MyLineChart::abstandGeaendert,
                   this,             &setAbstand );

QObject::connect(  ui->graphicsView, &MyLineChart::linienlaengeGeaendert,
                   this,             &setLinienlaenge );
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

void MainWindow::setAbstand(float abstand)
{

    QString a = QString::number(abstand);

     ui->abstand->setText(a);
}


void MainWindow::setLinienlaenge(float abstand)
{
    QString a = QString::number(abstand);

     ui->linienlaenge->setText(a);
}

void MainWindow::on_pushButton_4_clicked()
{
    showTestdaten(VIERTAKT);
}


void MainWindow::on_pushButton_5_clicked()  //Test daten
{
    showTestdaten(LINEAR);
}

void MainWindow::on_pushButton_6_clicked()  // Punkte vermesung
{
    showPunktevermessung();
}
