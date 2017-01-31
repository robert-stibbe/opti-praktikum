#include "mylinechart.h"
#include <QtCore/qmath.h>
#include <QtCore/QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

MyLineChart::MyLineChart()
{

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
     qDebug() << "linienabstand: " << linienabstand;

     /*
     //entferne bisherige linien
         QList<QAbstractSeries *> oldseriesList = oldChart->series();
         foreach (QAbstractSeries *series, oldseriesList)
         {
            oldChart->removeSeries(series);
         }

*/
     QList<QPointF> zlist;

     zlist.append(pointR);
     zlist.append(pointL);




 //füge neue punkte hinzu
     QLineSeries *lin1 = new QLineSeries();


     lin1->append(2, 2);
     lin1->append(3, 8);
   //  lin1->append(10, 10);

     /*
     lin1->append(2, 2);
     lin1->append(3, 8);
     lin1->append(7, 4);
     */
    // lin1->append(10, 5);

    // lin1->append(zlist);
  //   chart()->addSeries(lin1);

  qDebug() << "Linie: " << zlist;
   //  repaint();



    event->accept(); // A no-op, but I will do it anyway!!!
}

