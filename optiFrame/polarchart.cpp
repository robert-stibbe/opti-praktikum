#include "polarchart.h"
#include <QtGui/QMouseEvent>
#include <QtCore/QDebug>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QTime>
#include <QTimer>
#include "path_reduction.h"

QT_CHARTS_USE_NAMESPACE

static const qreal MITTELWERT = 0.0;
static const qreal increment  = 0.0001;
static const qreal ABWEICHUNG = 3 / std::sqrt(increment);
static const qreal epsilon = 0.15;
static const qreal epsilon2 = 1.0;

long frameZaehler = 0;

PolarChart::PolarChart(QWidget *parent)
    : QChartView(parent)
{
    druckWerte = new QLineSeries();
    isOriSichtbar=true;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1);
}

void PolarChart::update(QValueAxis *angularAxis, QValueAxis *radialAxis)
{
    static std::random_device rd;
    static std::mt19937 e2(rd());
    static std::normal_distribution<qreal> dist(MITTELWERT, ABWEICHUNG);

    switchChartType();

//addiere zufallszahlen zu Druckwert
    QList<QPointF> zlist;
    qreal y1 = 0;
    qreal y2 = 0;
    foreach (QPointF pt, oriWerte)
    {
       qreal zufallsZahl = dist(e2);
       //  qDebug() << zufallsZahl;
       const qreal factor = increment;
       y1 = ( pt.ry()+zufallsZahl ) * factor + y1 * (1-factor);
       y2 = ( y1                  ) * factor + y2 * (1-factor);
       zlist.append(QPointF( pt.rx(),y2 ) );
    }

    QTime t;
    t.start();
   QVector<QPointF> reducewerte1 = reducePathDouglasPeucker( zlist.toVector(),  epsilon ); //O1
    QVector<QPointF> reducewerte2 = reducePathLang( zlist.toVector(),  epsilon );  //O2
    QVector<QPointF> reducewerte3 = reducePathRalph( zlist.toVector(),  epsilon ); //O3
//    QVector<QPointF> reducewerte4 = reducePathDouglasPeucker( reducePathRalph( zlist.toVector(),  epsilon/2 ), epsilon/2 );   //O4
//    QVector<QPointF> reducewerte5 = reducePathDouglasPeucker( reducePathLang( zlist.toVector(),  epsilon/2 ), epsilon/2 ); //O5
   //qDebug() << reducewerte.size()   << " reduzierte Punkte. Benoetigte Zeit: " << t.elapsed();
/*
    t.start();
    QVector<QPointF> reducewerte2 = reducePathDouglasPeucker( reducePathLang( zlist.toVector(),  epsilon2/2 ), epsilon2/2 );
    qDebug() << reducewerte2.size() << "epsilon = " << epsilon2 << " reduzierte Punkte. Benoetigte Zeit: " << t.elapsed();

    t.start();
    QVector<QPointF> reducewerte3 = reducePath( zlist.toVector(),  8 );
    qDebug() << reducewerte3.size() << "epsilon = 8 reduzierte Punkte. Benoetigte Zeit: " << t.elapsed();
*/
    /*
    //Orginalwerte
    druckWerte->attachAxis(radialAxis);
    druckWerte->attachAxis(angularAxis);
    druckWerte->replace(zlist);
    druckWerte->setName("ori");
//    chart()->addSeries(druckWerte);
*/
    //reduzierte punkte
    reduzierteDruckwerte1 = new QLineSeries();
    reduzierteDruckwerte1->attachAxis(radialAxis);
    reduzierteDruckwerte1->attachAxis(angularAxis);
    reduzierteDruckwerte1->replace(reducewerte1.toList());
    reduzierteDruckwerte1->setName(QString("O1, epsilon = %1").arg(epsilon));
    chart()->addSeries(reduzierteDruckwerte1);

    //anders reduzierte punkte
   reduzierteDruckwerte2 = new QLineSeries();
   reduzierteDruckwerte2->attachAxis(radialAxis);
   reduzierteDruckwerte2->attachAxis(angularAxis);
   reduzierteDruckwerte2->replace(reducewerte2.toList());
   reduzierteDruckwerte2->setName(QString("O2, epsilon = %1").arg(epsilon));
   chart()->addSeries(reduzierteDruckwerte2);

    //ganz anders reduzierte punkte
   reduzierteDruckwerte3 = new QLineSeries();
   reduzierteDruckwerte3->attachAxis(radialAxis);
   reduzierteDruckwerte3->attachAxis(angularAxis);
   reduzierteDruckwerte3->replace(reducewerte3.toList());
   reduzierteDruckwerte3->setName(QString("O3, epsilon = %1").arg(epsilon));
   chart()->addSeries(reduzierteDruckwerte3);

   // repaint();
    qDebug() << "Frame: " << ++frameZaehler;
}

void PolarChart::initBasisWerte(QValueAxis *angularAxis, QValueAxis *radialAxis)
{
    QLineSeries *marker1 = new QLineSeries();
    marker1->append(0, 0);
    marker1->append(90, 100);

// erzeuge testpunkte
  for (float phi=0; phi<360; phi+=increment)
  {
      float druck = phi/3.6;
      oriWerte.append( QPointF(phi, druck));
  }
  qDebug() << oriWerte.size() << " Punkte generiert!";



  /*
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
  */
/*
  druckWerte2->append(oriWerte);
//  druckWerte2->append(reducewerte.toList());

  chart()->addSeries(druckWerte2);
  druckWerte2->attachAxis(radialAxis);
  druckWerte2->attachAxis(angularAxis);
*/
}

void PolarChart::keyPressEvent(QKeyEvent *event)
{

    switch (event->key())
    {
    case Qt::Key_1:
        if (isOriSichtbar )
        {
             chart()->removeSeries(druckWerte);
             isOriSichtbar=false;
        }
        else
        {
            chart()->addSeries(druckWerte);
            isOriSichtbar=true;
        }
        switchChartType();
        switchChartType();
        break;

    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-8.0, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(8.0, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 8.0);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -8.0);
        break;
    case Qt::Key_Space:
        switchChartType();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}
void PolarChart::switchChartType()
{
    QChart *newChart;
    QChart *oldChart = chart();

    if (oldChart->chartType() == QChart::ChartTypeCartesian)
        newChart = new QPolarChart();
    else
        newChart = new QChart();

    // Move series and axes from old chart to new one
    QList<QAbstractSeries *> seriesList = oldChart->series();
    QList<QAbstractAxis *> axisList = oldChart->axes();
    QList<QPair<qreal, qreal> > axisRanges;

    foreach (QAbstractAxis *axis, axisList) {
        QValueAxis *valueAxis = static_cast<QValueAxis *>(axis);
        axisRanges.append(QPair<qreal, qreal>(valueAxis->min(), valueAxis->max()));
    }

    foreach (QAbstractSeries *series, seriesList)
        oldChart->removeSeries(series);

    foreach (QAbstractAxis *axis, axisList) {
        oldChart->removeAxis(axis);
        newChart->addAxis(axis, axis->alignment());
    }


    foreach (QAbstractSeries *series, seriesList) {
        newChart->addSeries(series);
        foreach (QAbstractAxis *axis, axisList)
            series->attachAxis(axis);
    }

    int count = 0;
    foreach (QAbstractAxis *axis, axisList) {
        axis->setRange(axisRanges[count].first, axisRanges[count].second);
        count++;
    }

    newChart->setTitle(oldChart->title());
    setChart(newChart);
    delete oldChart;
}
