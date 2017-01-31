#include "mainwindow.h"
#include "mylinechart.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>


QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // QtCharts::QChart

     QLineSeries *series = new QLineSeries();
     series->append(0, 0);
     series->append(10, 10);

     QChart *chart = new QChart();
     chart->legend()->hide();
     chart->addSeries(series);
     chart->createDefaultAxes();
     chart->setTitle("Simple line chart example");




//     MyLineChart *chartView = new MyLineChart();
  //   chartView->setChart(chart);

    ui->graphicsView->setChart(chart);

}

MainWindow::~MainWindow()
{
    delete ui;
}


