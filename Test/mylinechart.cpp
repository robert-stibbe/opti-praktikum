#include "mylinechart.h"
#include <QtCore/qmath.h>
#include <QtCore/QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QScatterSeries>

MyLineChart::MyLineChart()
{
    lin1 = new QLineSeries();
}

MyLineChart::MyLineChart(QWidget *parent )
{

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

    qreal linienabstand=  qSqrt(qPow(pointL.rx() - pointR.rx(), 2) + qPow(pointL.ry() - pointR.ry(), 2));
   // qDebug() << "linienabstand: " << linienabstand;

    emit setAbstand( linienabstand );
     //entferne bisherige linien
         QList<QAbstractSeries *> oldseriesList = oldChart->series();
         foreach (QAbstractSeries *series, oldseriesList)
         {
            oldChart->removeSeries(series);
         }

 //    QList<QPointF> zlist;
 //    zlist.append(pointR);
 //    zlist.append(pointL);

    QLineSeries *lin1= new QLineSeries();
    QLineSeries *lin2= new QLineSeries();
 //füge neue punkte hinzu
     lin1->clear();

     QScatterSeries *series1 = new QScatterSeries();
     series1->append(0,0);
     series1->append(10,10);

     lin1->append(0, 0);
     lin1->append(pointR.rx(), pointR.ry());
     lin1->append(pointL.rx(), pointL.ry());
     lin1->append(10, 10);

     /*
     lin1->append(2, 2);
     lin1->append(3, 8);
     lin1->append(7, 4);
     */
    // lin1->append(10, 5);

    // lin1->append(zlist);
     chart()->addSeries(series1);

    chart()->addSeries(lin1);
    chart()->addSeries(lin2);

 qDebug() << "Linie: " << lin1->points();
     repaint();
   // update();

    event->accept(); // A no-op, but I will do it anyway!!!
}

