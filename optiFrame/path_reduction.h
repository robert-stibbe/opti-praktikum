#pragma once

#include <QVector>
#include <QPointF>

QVector<QPointF> reducePathDouglasPeucker(
    const QVector<QPointF> & points,
    const qreal epsilon
    );

QVector<QPointF> reducePathLang(
    const QVector<QPointF> & points,
    const qreal epsilon
    );

QVector<QPointF> reducePathRalph(
    const QVector<QPointF> & points,
    const qreal epsilon
    );
