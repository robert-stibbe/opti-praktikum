#include "mainwindow.h"
#include "mylinechart.h"
#include "ui_mainwindow.h"
#include <QtCore/QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include <QMessageBox>
#include <QPolarChart>
#include <QScatterSeries>
#include <QTimer>
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

void MainWindow::showAchsen()
{
    const qreal angularMin = 0;
    const qreal angularMax = 360;

    const qreal radialMin = 0;
    const qreal radialMax = 100;

    QPolarChart *chart = new QPolarChart();

   // chart->addSeries(druckWerte);
  //  chart->setTitle("Leertaste zum Wechseln.");

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.

    angularAxis->setLabelFormat(("%.1f Grad")); //%.0f \u00B0
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(10);
    radialAxis->setLabelFormat("%d bar");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    radialAxis->setRange(radialMin, radialMax);
    angularAxis->setRange(angularMin, angularMax);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setChart(chart);
}

void MainWindow::showTestdaten(Dtyp datentyp)
{

    ui->graphicsView->stopDenBloedenTimer();
    ui->stackedWidget->setCurrentIndex(2);
    showAchsen();

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
        for (int winkel = 0; winkel <= 360; winkel += 5)
           {
              // int radius =  (winkel / radialMax) * radialMax;
              // int radius = (1 + sin(winkel / 360.0 * 2*3.141592653589793238 ) ) / 2 * (radialMax - radialMin) +radialMin;
              int radius = winkel;
              oriWerte.append(QPointF (winkel, radius));
           }
    }
    QLineSeries *druckWerte = new  QLineSeries();
    druckWerte->append(oriWerte);
    ui->graphicsView->chart()->addSeries(druckWerte);
}

void MainWindow::showPunktevermessung()
{
// Verbinde Signal und Slot für Abstand und Linienlänge
    QObject::connect(  ui->graphicsView, &MyLineChart::abstandGeaendert,
                       this,             &setAbstand );

    QObject::connect(  ui->graphicsView, &MyLineChart::linienlaengeGeaendert,
                       this,             &setLinienlaenge );

    QChart *chart = new QChart(); // Erzeuge Liniendiagramm
    chart->legend()->hide();
    ui->graphicsView->setChart(chart); // ui graphicsView ist vom Typ MyLineCharft

    ui->graphicsView->initMyChart();
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


void MainWindow::on_pushButton_5_clicked()  //Testdaten
{
    showTestdaten(LINEAR);
}

void MainWindow::on_pushButton_6_clicked()  // Punktevermessung
{
    ui->stackedWidget->setCurrentIndex(3);
    showPunktevermessung();
}

void MainWindow::on_diagrammtypUmschalter_clicked()
{
    ui->graphicsView->switchChartType();
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    showAchsen();
    ui->graphicsView->initBasisWerte();

    //connect(timer, SIGNAL(timeout()), this, SLOT(ui->graphicsView->update()));
}

void MainWindow::on_pushButton_8_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_9_clicked()
{
     QString s = ui->schrittweite1->text();
     bool ok;
     double sw =  s.toFloat(&ok);
     if (ok)
     {
         if (sw > 0.0008 && sw < 20 )
         {
            ui->graphicsView->setSchrittweite( sw );
            qDebug() << "Gueltige schrittweite " << sw;
         }
         else
         {
             QMessageBox messageBox;
             messageBox.critical(0,"Error","Schrittweite ungueltig !");
             messageBox.setFixedSize(500,200);
             messageBox.show();
         }
     }
     else
     {
         qDebug() << "schrittweite falsch " << sw;
     }
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->graphicsView->switchTimer();
    ui->graphicsView->initBasisWerte();
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->graphicsView->swichReduceType(MyLineChart::Ralph);
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->graphicsView->swichReduceType(MyLineChart::DouglasPeucker);
}

void MainWindow::on_pushButton_13_clicked()
{
    ui->graphicsView->swichReduceType(MyLineChart::Unveraendert);
}
