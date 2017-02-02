#include "path_reduction.h"

#include <QLineF>
#include <QVector2D>

#include <iterator>


static qreal abstand( const QLineF & line,
                      const QPointF & point )
{
  const auto l = QVector2D( line.p2() - line.p1() );
  const auto p = QVector2D( point - line.p1() );
  const auto pl = QVector2D::dotProduct( p, l );
  if ( pl < 0 )
    return p.length();
  if ( QVector2D::dotProduct( p-l, l ) > 0 )
    return (p-l).length();
  return sqrt( p.lengthSquared() - pl*pl/l.lengthSquared() );
}


static void reducePathImpl( QVector<QPointF> & out,
                            const QPointF * const points,
                            const int nPoints,
                            const qreal epsilon )
{
  if ( nPoints <= 2 )
  {
    return;
  }

  const QLineF line = { points[0], points[nPoints-1] };
  auto maxAbstandIndex = 0;
  qreal maxAbstand = 0;
  for ( auto i = 1; i < nPoints-1; ++i )
  {
    const auto d = abstand( line, points[i] );
    if ( d <= maxAbstand )
      continue;
    maxAbstand = d;
    maxAbstandIndex = i;
  }
  if ( maxAbstand <= epsilon )
    return;
  reducePathImpl( out, points, maxAbstandIndex, epsilon );
  out.push_back( points[maxAbstandIndex] );
  reducePathImpl( out, points+maxAbstandIndex, nPoints - maxAbstandIndex, epsilon );
}


QVector<QPointF> reducePath( const QVector<QPointF> & points,
                             qreal epsilon )
{
  if ( points.size() <= 2 )
    return points;

  QVector<QPointF> result;
  result.push_back( points.front() );
  reducePathImpl( result, &points.front(), points.size(), epsilon );
  result.push_back( points.back() );
  return result;
}
