#include "path_reduction.h"

#include <QLineF>
#include <QVector2D>

#include <cassert>
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
  return std::sqrt( std::abs( p.lengthSquared() - pl*pl/l.lengthSquared()));
}


static void reducePathDouglasPeuckerImpl(
    QVector<QPointF> & out,
    const QPointF * const points,
    const int nPoints,
    const qreal epsilon
    )
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
  reducePathDouglasPeuckerImpl( out, points, maxAbstandIndex+1, epsilon );
  out.push_back( points[maxAbstandIndex] );
  reducePathDouglasPeuckerImpl( out, points+maxAbstandIndex, nPoints - maxAbstandIndex, epsilon );
}


QVector<QPointF> reducePathDouglasPeucker(
    const QVector<QPointF> & points,
    const qreal epsilon
    )
{
  if ( points.size() <= 2 )
    return points;

  QVector<QPointF> result;
  result.push_back( points.front() );
  reducePathDouglasPeuckerImpl( result, &points.front(), points.size(), epsilon );
  result.push_back( points.back() );
  return result;
}


namespace // unnamed
{
  class ValidRegion
  {
  public:
    ValidRegion( float epsilon )
      : e( epsilon )
    {}

    bool isValid( const QVector2D & v ) const
    {
      const auto cc = c.lengthSquared();
      if ( cc <= e*e )
        return true;
      const auto vv = v.lengthSquared();
      if ( vv < cc )
        return false;
      QVector2D conj_c_v = { c.x()*v.x() + c.y()*v.y(),
                   std::abs( c.x()*v.y() - c.y()*v.x() ) };
      return conj_c_v.y() <= t * conj_c_v.x();
    }

    void update( const QVector2D v )
    {
      const auto cc = c.lengthSquared();
      const auto ee = e*e;
      const auto vv = v.lengthSquared();
      const auto dd = vv - ee;
      if ( cc <= ee )
      {
        if ( dd <= 0 )
          return;
        c = v;
        t = e / std::sqrt( dd );
        return;
      }
      const auto s = std::sqrt( ee / dd );
      const auto alDir = QVector2D{ c.x()-c.y()*t, c.x()*t+c.y() };
      const auto arDir = QVector2D{ c.x()+c.y()*t,-c.x()*t+c.y() };
      const auto dlDir = QVector2D{ v.x()-v.y()*s, v.x()*s+v.y() };
      const auto drDir = QVector2D{ v.x()+v.y()*s,-v.x()*s+v.y() };
      const auto isLeftToRight = []( auto a, auto b )
      {
        return a.x()*b.y() < a.y() * b.x();
      };
      const auto isAl = isLeftToRight( dlDir, alDir );
      const auto isAr = isLeftToRight( arDir, drDir );
      if ( !isAl && !isAr )
      {
        c = v;
        t = s;
        return;
      }
      if ( isAl && isAr )
      {
        c = c * std::sqrt( vv / cc );
        return;
      }
      const auto l = (isAl ? alDir : dlDir).normalized();
      const auto r = (isAr ? arDir : drDir).normalized();
      const auto newCDir = l+r;
      c = newCDir * sqrt( vv / newCDir.lengthSquared() );
      t = (l-r).length() / newCDir.length();
    }

    void reset()
    {
      t = 0;
      c = {};
    }

  private:
    const float e = 0;
    // If c.length() < e, then the value is tan(alpha/2), where alpha is
    // the opening angle of the angular region.
    // Otherwise the value of t is 0.
    float t = 0;
    QVector2D c;
  };
} // unnamed namespace


static void check(
    const QVector<QPointF> points,
    const QVector<int> idxs,
    const qreal epsilon
    )
{
  for ( int i = 1; i < idxs.size(); ++i )
  {
    const auto line = QLineF{ points[idxs[i-1]], points[idxs[i]] };
    for ( int j = idxs[i-1]+1; j < idxs[i]; ++j )
    {
      const auto dist = abstand( line, points[j] );
      assert( dist <= epsilon * 1.01 );
    }
  }
}


QVector<QPointF> reducePathLang(
    const QVector<QPointF> & points,
    const qreal epsilon
    )
{
  if ( points.size() <= 2 )
    return points;

  QVector<int> resultIdxs = { 0 };
  ValidRegion validRegion( epsilon );
  for ( auto candidatePointIdx = 1; candidatePointIdx < points.size() - 1; ++candidatePointIdx )
  {
    if ( !validRegion.isValid( QVector2D{ points[candidatePointIdx] - points[resultIdxs.back()] } ) )
    {
      resultIdxs.push_back( candidatePointIdx-1 );
      validRegion.reset();
    }
    validRegion.update( QVector2D{ points[candidatePointIdx] - points[resultIdxs.back()] } );
  }
  resultIdxs.push_back( points.size() - 1 );
  check( points, resultIdxs, epsilon );
  QVector<QPointF> result;
  result.reserve( resultIdxs.size() );
  for ( auto idx : resultIdxs )
    result.push_back( points[idx] );
  return result;
}


namespace // unnamed
{
  struct LineInfo
  {
    LineInfo(
        int A_,
        int B_,
        double accumulatedDist_ )
      : A(A_)
      , B(B_)
      , accumulatedDist(accumulatedDist_)
    {
      assert( A < B );
    }

    int A = 0;
    int B = 0;
    qreal accumulatedDist = 0;
  };
} // unnamed namespace

static LineInfo combine(
    const LineInfo & K,
    const LineInfo & L,
    const QVector<QPointF> & points )
{
  assert( K.B == L.A );
  const auto line = QLineF{ points[K.A], points[L.B] };
  const auto point = points[K.B];
  return
  {
    K.A, L.B,
    std::max( K.accumulatedDist, L.accumulatedDist ) + abstand( line, point )
  };
}


QVector<QPointF> reducePathRalph(
    const QVector<QPointF> & points,
    const qreal epsilon
    )
{
  if ( points.size() <= 2 )
    return points;

  std::vector<LineInfo> linesIn;
  linesIn.reserve( points.size() );
  for ( int i = 1; i < points.size(); ++i )
    linesIn.emplace_back( i-1, i, 0 );

  std::vector<LineInfo> linesOut;
  linesOut.reserve( linesIn.size() );
  do
  {
    linesOut.clear();
    std::size_t i = 0;
    for ( ; i < linesIn.size()-1; ++i )
    {
      const auto line = combine( linesIn[i], linesIn[i+1], points );
      if ( line.accumulatedDist > epsilon )
      {
        linesOut.push_back( linesIn[i] );
        continue;
      }
      linesOut.push_back( line );
      ++i;
    }
    if ( i == linesIn.size() - 1 )
      linesOut.push_back( linesIn.back() );
    linesIn.swap( linesOut );
  } while ( linesIn.size() < 3 * linesOut.size() / 4 );
  QVector<QPointF> result;
  result.reserve( linesIn.size() + 1 );
  for ( const auto & line : linesIn )
    result.push_back( points[line.A] );
  result.push_back( points.back() );
  return result;
}
