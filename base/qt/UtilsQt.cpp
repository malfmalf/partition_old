#include "UtilsQt.h"
#include "Geometry.h"
#include <assert.h>

QLineF 
clip(QRectF r, QLineF l,qreal rot,QPointF pos){
    QTransform trans;
    trans.rotate(rot);
    l = trans.map(l);
    l.translate(pos);
    QPointF p1,p2;
    if(l.intersect(QLineF(r.topLeft(),r.topRight()),&p1)==QLineF::BoundedIntersection){
        if(l.intersect(QLineF(r.topRight(),r.bottomRight()),&p2)==QLineF::BoundedIntersection) return QLineF(p1,p2);
        if(l.intersect(QLineF(r.bottomRight(),r.bottomLeft()),&p2)==QLineF::BoundedIntersection) return QLineF(p1,p2);
        if(l.intersect(QLineF(r.bottomLeft(),r.topLeft()),&p2)==QLineF::BoundedIntersection) return QLineF(p1,p2);
        if(r.contains(l.p1())) return QLineF(l.p1(),p1);
        if(r.contains(l.p2())) return QLineF(p1,l.p2());
    }
    else if(l.intersect(QLineF(r.topRight(),r.bottomRight()),&p1)==QLineF::BoundedIntersection){
        if(l.intersect(QLineF(r.bottomRight(),r.bottomLeft()),&p2)==QLineF::BoundedIntersection) return QLineF(p1,p2);
        if(l.intersect(QLineF(r.bottomLeft(),r.topLeft()),&p2)==QLineF::BoundedIntersection) return QLineF(p1,p2);
        if(r.contains(l.p1())) return QLineF(l.p1(),p1);
        if(r.contains(l.p2())) return QLineF(p1,l.p2());
    }
    else if(l.intersect(QLineF(r.bottomRight(),r.bottomLeft()),&p1)==QLineF::BoundedIntersection){
        if(l.intersect(QLineF(r.bottomLeft(),r.topLeft()),&p2)==QLineF::BoundedIntersection) return QLineF(p1,p2);
        if(r.contains(l.p1())) return QLineF(l.p1(),p1);
        if(r.contains(l.p2())) return QLineF(p1,l.p2());
    }
    else if(l.intersect(QLineF(r.bottomLeft(),r.topLeft()),&p1)==QLineF::BoundedIntersection){
        if(r.contains(l.p1())) return QLineF(l.p1(),p1);
        if(r.contains(l.p2())) return QLineF(p1,l.p2());
    }
    return l;
}

void 
drawCenteredImagePortion(QPainter& painter,QImage image,qreal x,qreal y,qreal maxWidth){
    if(maxWidth<=0.0)          maxWidth = image.width();
    if(maxWidth>image.width()) maxWidth = image.width();
    //     x-=image.width()*0.5;
    y-=image.height()*0.5;
    painter.drawImage(QRect(x,y,maxWidth,image.height()),image,QRect(0,0,maxWidth,image.height()));
}

void 
drawCenteredImagePortionBack(QPainter& painter,QImage image,qreal x,qreal y,qreal maxWidth){
    if(maxWidth<=0.0)          maxWidth = image.width();
    if(maxWidth>image.width()) maxWidth = image.width();
    //     x-=image.width()*0.5;
    y-=image.height()*0.5;
    painter.drawImage(QRect(x-maxWidth,y,maxWidth,image.height()),image.mirrored(true),QRect(0,0,maxWidth,image.height()));
}

void 
drawCenteredImage(QPainter& painter,QImage image,qreal x,qreal y){
    x-=image.width()*0.5;
    y-=image.height()*0.5;
    painter.drawImage(QRect(x,y,image.width(),image.height()),image,QRect(0,0,image.width(),image.height()));
}

std::string 
qstring2string(const QString& s){
    QByteArray arr = s.toAscii();
    return std::string(arr.constBegin(),arr.constEnd());
}

point2d_t 
toPoint(QPointF p){
    return p;
}

cLine 
toLine(QLineF l){
    return cLine(l.p1(),l.p2());
}
cPolygon 
toPolygon(QRectF r){
    cPolygon pol;
    pol.addPoint(r.topLeft()    );
    pol.addPoint(r.topRight()   );
    pol.addPoint(r.bottomRight());
    pol.addPoint(r.bottomLeft() );
    return pol;
}

cPolygon 
toPolygon(QPolygonF pol){
    cPolygon result;
    if(pol.empty()) return result;
    point2d_t center;
    size_t count = pol.isClosed() ? pol.size() -1 : pol.size();
    for(size_t i = 0;i<count;++i){
        result.addPoint(pol[i]);
    }
    return result;
}

QPointF 
toQPoint(point2d_t p){
    return p;
}

QLineF 
toQLine(const cLine& l){
    return QLineF(l.p1(),l.p2());
}

QPolygonF 
toQPolygon(const cPolygon& pol){
    QPolygonF result;
    if(!pol.isValid()) return result;
    for(size_t i = 0;i<pol.numPoints();++i){
        result.push_back(pol.points()[i]);
    }
    result.push_back(pol.points()[0]);
    return result;
}

