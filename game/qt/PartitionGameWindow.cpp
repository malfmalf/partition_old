#include "PartitionGameWindow.h"
#include "UtilsQt.h"

PartitionGameWindow::PartitionGameWindow(QWidget* parent/* = NULL*/) : QWidget(parent){
    setGameWidth(300);
    setGameHeight(300);
}

void 
PartitionGameWindow::step(void){

}

void 
PartitionGameWindow::reset(void){
    mLines.clear();
    QPointF center(width()/2,height()/2);
    mLines.push_back(cLine(point2d_t(center.x()-mGameWidth/2,center.y()-mGameHeight/2),point2d_t(center.x()+mGameWidth/2,center.y()-mGameHeight/2)));
    mLines.push_back(cLine(point2d_t(center.x()+mGameWidth/2,center.y()-mGameHeight/2),point2d_t(center.x()+mGameWidth/2,center.y()+mGameHeight/2)));
    mLines.push_back(cLine(point2d_t(center.x()+mGameWidth/2,center.y()+mGameHeight/2),point2d_t(center.x()-mGameWidth/2,center.y()+mGameHeight/2)));
    mLines.push_back(cLine(point2d_t(center.x()-mGameWidth/2,center.y()+mGameHeight/2),point2d_t(center.x()-mGameWidth/2,center.y()-mGameHeight/2)));
    cPolygon poly;
    poly.addPoint(point2d_t(center.x()-mGameWidth/2,center.y()-mGameHeight/2));
    poly.addPoint(point2d_t(center.x()+mGameWidth/2,center.y()-mGameHeight/2));
    poly.addPoint(point2d_t(center.x()+mGameWidth/2,center.y()+mGameHeight/2));
    poly.addPoint(point2d_t(center.x()-mGameWidth/2,center.y()+mGameHeight/2));
    mPolygons.clear();
    mPolygons.push_back(poly);
    QMouseEvent ev(QEvent::MouseButtonPress,(center+QPointF(0.0,10.0)).toPoint(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mousePressEvent(&ev);
    QMouseEvent ev2(QEvent::MouseButtonRelease,(center+QPointF(10.0,10.0)).toPoint(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mouseReleaseEvent(&ev2);
    QMouseEvent ev3(QEvent::MouseButtonPress,(center+QPointF(10.0,20.0)).toPoint(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mousePressEvent(&ev3);
    QMouseEvent ev4(QEvent::MouseButtonRelease,(center+QPointF(10.0,30.0)).toPoint(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mouseReleaseEvent(&ev4);
    QMouseEvent ev5(QEvent::MouseButtonPress,(center+QPointF(-80.0,60.0)).toPoint(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mousePressEvent(&ev5);
    QMouseEvent ev6(QEvent::MouseButtonRelease,(center+QPointF(-81.0,30.0)).toPoint(),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    mouseReleaseEvent(&ev6);
    update();
}

void 
PartitionGameWindow::setNumBalls(int b){
    mNumBalls = b;
}

void 
PartitionGameWindow::setSpeed(double s){
    mSpeed = s;
}

void 
PartitionGameWindow::setGameWidth(int w){
    mGameWidth = w;
    reset();
}

void 
PartitionGameWindow::setGameHeight(int h){
    mGameHeight = h;
    reset();
}

void 
PartitionGameWindow::mousePressEvent(QMouseEvent* event){
    mDrawingLine = true;
    mCurrentLine.setP1(toPoint(event->posF()));
    mCurrentLine.setP2(toPoint(event->posF()));
    update();
}

void 
PartitionGameWindow::mouseMoveEvent(QMouseEvent* event){
    mCurrentLine.setP2(toPoint(event->posF()));
    update();
}

void 
PartitionGameWindow::mouseReleaseEvent(QMouseEvent* event){
    mCurrentLine.setP2(toPoint(event->posF()));

    mDrawingLine = false;
    update();
    if(!clipLine(mCurrentLine)) return;

    mLines.push_back(mCurrentLine);

    point2d_t medio(mCurrentLine.p1()*0.5+mCurrentLine.p2()*0.5);

    for(tPolygonList::iterator it = mPolygons.begin();it!=mPolygons.end();++it){
        if(it->isInside(medio)){
            cLine l(medio,medio+normalized(mCurrentLine.dir()));
            cPolygon pl1,pl2;
            if(!it->bisect(l,pl1,pl2)){
                qDebug()<<"FALLO DE BISECCION!!!!"<<endl;
                it->bisect(l,pl1,pl2);
                return;
            }
            mPolygons.erase(it);
            mPolygons.push_back(pl1);
            mPolygons.push_back(pl2);
            break;
        }
    }
}

void 
PartitionGameWindow::paintEvent(QPaintEvent* event){
    QPainter pt(this);
    pt.setRenderHint(QPainter::Antialiasing);
    pt.drawText(10,10,QString().sprintf("%d x %d lines = %d balls = %d speed = %f",mGameWidth,mGameHeight,mLines.size(),mNumBalls,mSpeed));
    paintAreas(pt);
    paintLines(pt);
    paintCurrentLine(pt);
}

void 
PartitionGameWindow::paintLines(QPainter &pt){
    for(size_t i = 0;i<mLines.size();++i){
        pt.setPen(QPen(QBrush(QColor("red")),5,Qt::SolidLine,Qt::RoundCap));
        pt.drawLine(toQLine(mLines[i]));
    }
    for(size_t i = 0;i<mLines.size();++i){
        pt.setPen(QPen(QBrush(QColor("yellow")),2));
        pt.drawLine(toQLine(mLines[i]));
    }
}

void 
PartitionGameWindow::paintCurrentLine(QPainter &pt){
    if(mDrawingLine){
        cLine l(mCurrentLine);
        if(clipLine(l)){
            pt.setPen(QPen(QBrush(QColor("green")),5,Qt::SolidLine,Qt::RoundCap));
            pt.drawLine(toQLine(l));
            pt.setPen(QPen(QBrush(QColor("yellow")),2));
            pt.drawLine(toQLine(l));
        }
        pt.setPen(QPen(QBrush(QColor("black")),1));
        pt.drawLine(toQLine(mCurrentLine));
    }
}

void 
PartitionGameWindow::paintAreas(QPainter &pt){
    QVector<QColor> colors;
    colors.push_back(QColor(255,0,0,128));
    colors.push_back(QColor(255,255,0,128));
    colors.push_back(QColor(255,0,255,128));
    colors.push_back(QColor(0,255,0,128));
    colors.push_back(QColor(0,0,255,128));
    colors.push_back(QColor(0,255,255,128));
    int cidx = 0;
    pt.setPen(Qt::NoPen);
    for(tPolygonList::const_iterator it = mPolygons.begin();it!=mPolygons.end();++it){
        pt.setBrush(QColor(colors[cidx]));
        pt.drawPolygon(toQPolygon(*it));
        ++cidx;
        if(cidx>=colors.size()) cidx = 0;
    }
}

bool 
PartitionGameWindow::clipLine(cLine& l){
    cLine other(l.p1(),l.p1()-l.dir());
    point2d_t p1,p2;
    bool r1 = ray_clip(l,mLines,p2);
    bool r2 = ray_clip(other,mLines,p1);
    if(distanceSquared(p1,p2)<1.0) return false;
    for(size_t i = 0;i<mLines.size();++i){
        number_t d1 = distance(mLines[i],l.p1());
        number_t d2 = distance(mLines[i],l.p2());
        if(d1<1.0 && d2<1.0) return false;
    }

    if(!r1 || !r2) return false;
    l.set(p1,p2);
    return true;
}
