#pragma once
#include <QtGui>
#include <string>
#include "Geometry.h"

std::string qstring2string(const QString& s);
QLineF clip(QRectF r, QLineF l,qreal rot,QPointF pos);
void drawCenteredImagePortion(QPainter& painter,QImage image,qreal x,qreal y,qreal maxWidth);
void drawCenteredImagePortionBack(QPainter& painter,QImage image,qreal x,qreal y,qreal maxWidth);
void drawCenteredImage(QPainter& painter,QImage image,qreal x,qreal y);

point2d_t toPoint(QPointF p);
cLine     toLine(QLineF l);
cPolygon  toPolygon(QRectF r);
cPolygon  toPolygon(QPolygonF pol);

QPointF   toQPoint(point2d_t p);
QLineF    toQLine(const cLine& l);
QPolygonF toQPolygon(const cPolygon& pol);
