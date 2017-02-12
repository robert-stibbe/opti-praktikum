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

static const bool  FRAME_TIMER_AKTIV = false; // true für frame messung
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
    reduzierteDruckwerte1 = new QLineSeries();
    reduzierteDruckwerte2 = new QLineSeries();
    reduzierteDruckwerte3 = new QLineSeries();

    isOriSichtbar=true;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    if (FRAME_TIMER_AKTIV)
       timer->start(1);
}

// anzeige von neuen Zufallskurven
void PolarChart::update(QValueAxis *angularAxis, QValueAxis *radialAxis)
{
    static std::random_device rd;
    static std::mt19937 e2(rd());
    static std::normal_distribution<qreal> dist(MITTELWERT, ABWEICHUNG);

    switchChartType();

//Addiere Zufallszahlen zu Druckwerten
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
    //Orginalwerte anzeigen
    druckWerte->attachAxis(radialAxis);
    druckWerte->attachAxis(angularAxis);
    druckWerte->replace(zlist);
    druckWerte->setName("ori");
    chart()->addSeries(druckWerte);
*/
    //reduzierte punkte
    reduzierteDruckwerte1->setName(QString("O1, epsilon = %1").arg(epsilon));
    reduzierteDruckwerte1->replace(reducewerte1.toList());
   // reduzierteDruckwerte1->attachAxis(radialAxis);
    //reduzierteDruckwerte1->attachAxis(angularAxis);
    chart()->addSeries(reduzierteDruckwerte1);
/*
    //anders reduzierte punkte
   reduzierteDruckwerte2->setName(QString("O2, epsilon = %1").arg(epsilon));
   reduzierteDruckwerte2->attachAxis(radialAxis);
   reduzierteDruckwerte2->attachAxis(angularAxis);
   reduzierteDruckwerte2->replace(reducewerte2.toList());
   chart()->addSeries(reduzierteDruckwerte2);

    //ganz anders reduzierte punkte
   reduzierteDruckwerte3->setName(QString("O3, epsilon = %1").arg(epsilon));
   reduzierteDruckwerte3->attachAxis(radialAxis);
   reduzierteDruckwerte3->attachAxis(angularAxis);
   reduzierteDruckwerte3->replace(reducewerte3.toList());
   chart()->addSeries(reduzierteDruckwerte3);
*/
   // repaint();
    if (FRAME_TIMER_AKTIV)
         qDebug() << "Frame: " << ++frameZaehler;
}

void PolarChart::createMarker()
{
    QLineSeries *marker1 = new QLineSeries();
    marker1->setName(QString("Marker"));

    marker1->append(0, 0);
    marker1->append(90, 100);

    chart()->addSeries(marker1);
}

void PolarChart::initBasisWerte(QValueAxis *angularAxis, QValueAxis *radialAxis)
{

// erzeuge basis testpunkte
  for (float phi=0; phi<360; phi+=increment)
  {
      float druck = phi/3.6;
      oriWerte.append( QPointF(phi, druck));
  }
  qDebug() << oriWerte.size() << " Punkte generiert!";
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
