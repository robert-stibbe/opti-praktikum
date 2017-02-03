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
/*
qreal abstand (QLineF l, QPointF p )
{
    QPointF a = l.p1();
    QPointF b = l.p2();
    QPointF lVector = b - a ;
    QPointF pVector = p - a ;
    qreal bereich = (pVector.rx()* lVector.rx() + pVector.ry() * lVector.ry()); //lvector * pvector
    if (bereich <= 0)
    {
        return qSqrt(pVector.rx()* pVector.rx() + pVector.ry() * pVector.ry());
    }

   qreal lenvector (lVector.rx()* lVector.rx() + lVector.ry() * lVector.ry());
   if (bereich >= lenvector)
   {
       QPointF diff = pVector - lVector;
       return qSqrt(diff.rx()* diff.rx() + diff.ry() * diff.ry());

   }

  qreal pp = (p.rx()* p.rx() + p.ry() * p.ry());
  qreal lp = (p.rx()* lVector.rx() + p.ry() * lVector.ry());
  qreal ll = (lVector.rx()* lVector.rx() + lVector.ry() * lVector.ry());

  return qSqrt( pp - (lp*lp / ll ));


}
*/

void MyLineChart::mousePressEvent(QMouseEvent *event)
{
    QChart *oldChart = chart();

    QPointF chartPos = oldChart->mapToValue(event->localPos());
    qDebug() << "Pos: " << chartPos;

    if (event->button() == Qt::RightButton)
    {
        pointR = chartPos;
//        pointP = chartPos;
    }
    else if (event->button() == Qt::LeftButton)
    {
        pointL = chartPos;
//       pointP = chartPos;
    }
    else if (event->button() == Qt::BackButton)
    {
        pointP = chartPos;
    }


   // qDebug() << "linienabstand: " << linienabstand;
   qreal linienLen = punktAbstand(pointL,pointR);
   emit linienlaengeGeaendert( linienLen );

   QLineF linie (pointL, pointR);
   qreal punktvonLinie = abstand(linie, pointP );
   qDebug() << "abstand punkt von linie: " << punktvonLinie;
   emit abstandGeaendert( punktvonLinie );


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
 //füge neue punkte hinzu
     lin1->clear();

     QScatterSeries *series1 = new QScatterSeries();
     series1->append(0,0);
      series1->append(pointP.rx(), pointP.ry());
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

 qDebug() << "Linie: " << lin1->points();
     repaint();
   // update();

    event->accept(); // A no-op, but I will do it anyway!!!
}

