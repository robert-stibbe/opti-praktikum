#include "mylinechart.h"
#include <QtCore/qmath.h>
#include <QtCore/QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QPolarChart>
#include <QScatterSeries>
#include <QTimer>
#include <QValueAxis>

const int MITTELWERT = 0;
const int ABWEICHUNG = 5;

long frameZaehler = 0;

// Berechne Punktabstand(Linienlänge) und Abstand Linie zu Punkt
// Klick auf Chart setzt Linienpunkte und den Abstandspunkt
MyLineChart::MyLineChart()
{
}

MyLineChart::MyLineChart(QWidget *parent )
{
    timer = new QTimer(this);
    QObject::connect(  timer,            &QTimer::timeout,
                   this, &MyLineChart::update);
   schrittweite1 = 0.5;
   druckWerte2 = new QLineSeries ();
   QList<QPointF> zlist;
   zlist.append(QPointF(0,0));
   druckWerte2->append(zlist);
}

MyLineChart::initMyChart()
{
     timer->stop();
     lin1 = new QLineSeries();
     series1 = new QScatterSeries();

    /*
    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(10, 10);

    QScatterSeries *series1 = new QScatterSeries();
    series1->append(0,0);
    series1->append(10,10);


     chart->addSeries(series1);
*/
     QValueAxis *xAxis = new QValueAxis();
     xAxis->setTickCount(10);
     xAxis->setLabelFormat(("%.1f ")); //%.0f \u00B0
     xAxis->setShadesVisible(true);
     xAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
     xAxis->setRange(0, 10);
     lin1->attachAxis(xAxis);
     series1->attachAxis(xAxis);
     chart()->addAxis(xAxis, Qt::AlignLeft);

     QValueAxis *yAxis = new QValueAxis();
     yAxis->setTickCount(10);
     yAxis->setLabelFormat("%d ");
     yAxis->setRange(0, 10);
     lin1->attachAxis(yAxis);
     series1->attachAxis(yAxis);
     chart()->addAxis(yAxis, Qt::AlignBottom);

    //   chart->createDefaultAxes();
     chart()->setTitle("Klicke auf den Chart : linke und rechte Maustaste = linie backbutton der Maus = punkt ");

    //     MyLineChart *chartView = new MyLineChart();
    //   chartView->setChart(chart);

//     lin1->append(3,3);
//     lin1->append(4,4);
     chart()->addSeries(lin1);//leere liste
     chart()->addSeries(series1);//leere liste
}

qreal punktAbstand (QPointF pointL , QPointF pointR)
{
   qreal linienabstand=  qSqrt(qPow(pointL.rx() - pointR.rx(), 2) + qPow(pointL.ry() - pointR.ry(), 2));

   return linienabstand;
}

qreal abstand (QLineF l, QPointF p )
{
    QPointF a = l.p1();
    QPointF b = l.p2();
    QPointF lVector = b - a ;
    QPointF pVector = p - a ;

    if (QPointF::dotProduct(lVector, pVector) <= 0)
    {
        return qSqrt (QPointF::dotProduct(pVector, pVector));
    }
    if (QPointF::dotProduct(lVector, pVector) >= QPointF::dotProduct(lVector, lVector) )
    {
      QPointF diff = pVector - lVector;
      return qSqrt(QPointF::dotProduct(diff,diff));
    }

     qreal pp = QPointF::dotProduct(pVector, pVector) ;
     qreal lp = QPointF::dotProduct(lVector, pVector);
     qreal ll = QPointF::dotProduct(lVector, lVector);
    return   qSqrt(pp - (lp*lp/ll) );
}


void MyLineChart::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-1.0, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(1.0, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 1.0);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -1.0);
        break;
    case Qt::Key_Space:
        switchChartType();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void MyLineChart::mousePressEvent(QMouseEvent *event)
{
    QChart *oldChart = chart();

    QPointF chartPos = oldChart->mapToValue(event->localPos());
    qDebug() << "Pos: " << chartPos;

    if (event->button() == Qt::RightButton)
    {
        pointR = chartPos;
    }
    else if (event->button() == Qt::LeftButton)
    {
        pointL = chartPos;
    }
    else if (event->button() == Qt::BackButton || Qt::MiddleButton)
    {
        pointP = chartPos;
    }

   // qDebug() << "linienabstand: " << linienabstand;
   qreal linienLen = punktAbstand(pointL,pointR) * 10.;
   emit linienlaengeGeaendert( linienLen );

   QLineF linie (pointL, pointR);
   qreal punktvonLinie = abstand(linie, pointP ) * 10.;
   qDebug() << "abstand punkt von linie: " << punktvonLinie;
   emit abstandGeaendert( punktvonLinie );

   if (!pointL.isNull() && !pointR.isNull())
   {
     QList<QPointF> zlist;
     zlist.append(pointR);
     zlist.append(pointL);
     lin1->replace(zlist);
   }

   if (!pointP.isNull())
   {
       series1->clear();
      //  series1->append(0,0);
        series1->append(pointP.rx(), pointP.ry());
      //   series1->append(10,10);
     //  series1->replace(0, pointP);
   }

   qDebug() << "Linie: " << lin1->points();
   qDebug() << "PunktL" << pointL;
   qDebug() << "PunktR" << pointR;
    // repaint();
   // update();

}


void MyLineChart::update()
{
    static std::random_device rd;
    static std::mt19937 e2(rd());
    static std::normal_distribution<> dist(MITTELWERT, ABWEICHUNG);

//addiere zufallszahlen zu Druckwert
    QList<QPointF> zlist;
    foreach (QPointF pt, oriWerte)
    {
       double zufallsZahl = dist(e2);
       //  qDebug() << zufallsZahl;
       zlist.append(QPointF( pt.rx(),pt.ry()+zufallsZahl));
    }

    foreach (QAbstractSeries *series,  chart()->series())
        chart()->removeSeries(series);

//füge neue punkte hinzu

    druckWerte2 = new QLineSeries();

      druckWerte2->append(zlist);
    //druckWerte2->replace(zlist);
    chart()->addSeries(druckWerte2);

   // repaint();
   // qDebug() << "Frame: " << ++frameZaehler;
}

void MyLineChart::initBasisWerte()
{
    QLineSeries *marker1 = new QLineSeries();
    marker1->append(0, 0);
    marker1->append(90, 100);

// erzeuge testpunkte
    oriWerte.clear();

    qDebug() << "schrittweite " << schrittweite1;

    for (float phi=0; phi<360; phi+= schrittweite1)
    {
       float druck = phi;
       oriWerte.append( QPointF (phi, druck));
    }

    qDebug() << oriWerte.size() << " Punkte generiert!";
    timer->start(1);
}

void MyLineChart::switchChartType()
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

    foreach (QAbstractAxis *axis, axisList)
    {
        QValueAxis *valueAxis = static_cast<QValueAxis *>(axis);
        axisRanges.append(QPair<qreal, qreal>(valueAxis->min(), valueAxis->max()));
    }

    foreach (QAbstractSeries *series, seriesList)
        oldChart->removeSeries(series);

    foreach (QAbstractAxis *axis, axisList)
    {
        oldChart->removeAxis(axis);
        newChart->addAxis(axis, axis->alignment());
    }


    foreach (QAbstractSeries *series, seriesList)
    {
        newChart->addSeries(series);
        foreach (QAbstractAxis *axis, axisList)
            series->attachAxis(axis);
    }

    int count = 0;
    foreach (QAbstractAxis *axis, axisList)
    {
        axis->setRange(axisRanges[count].first, axisRanges[count].second);
        count++;
    }

    newChart->setTitle(oldChart->title());
    setChart(newChart);
    delete oldChart;
}

void MyLineChart::setSchrittweite(double sw)
{

    schrittweite1 = sw ;
    initBasisWerte ();
}

void MyLineChart::stopDenBloedenTimer()
{
    timer->stop();
}
