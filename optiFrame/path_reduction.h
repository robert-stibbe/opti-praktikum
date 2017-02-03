#pragma once

#include <QVector>
#include <QPointF>

QVector<QPointF> reducePath( const QVector<QPointF> & points, qreal epsilon );
