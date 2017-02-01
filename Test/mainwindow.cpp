#include "mainwindow.h"
#include "mylinechart.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include <QScatterSeries>
#include <QValueAxis>


QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Verbinde Signal und Slot
    QObject::connect(  ui->graphicsView, SIGNAL( setAbstand(float)) ,
                       this,             SLOT ( setAbstand(float))  );

    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(10, 10);

    QScatterSeries *series1 = new QScatterSeries();
    series1->append(0,0);
    series1->append(10,10);

     QChart *chart = new QChart();
     chart->legend()->hide();
     chart->addSeries(series1);

     QValueAxis *xAxis = new QValueAxis();
     xAxis->setTickCount(10);

     xAxis->setLabelFormat(("%.1f x")); //%.0f \u00B0
     xAxis->setShadesVisible(true);
     xAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
     chart->addAxis(xAxis, Qt::AlignLeft);

     QValueAxis *yAxis = new QValueAxis();
     yAxis->setTickCount(10);
     yAxis->setLabelFormat("%d y");
     chart->addAxis(yAxis, Qt::AlignBottom);
     yAxis->setRange(0, 10);
     xAxis->setRange(0, 10);


   //  chartView->setRenderHint(QPainter::Antialiasing);
  //   chart->createDefaultAxes();
     chart->setTitle("Klicke auf den Chart");

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
