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
    cLine other(mCurrentLine.p1(),mCurrentLine.p1()-mCurrentLine.dir());
    point2d_t p1,p2;
    bool r1 = ray_clip(mCurrentLine,mLines,p2);
    bool r2 = ray_clip(other,mLines,p1);

    if(r1&&r2) mLines.push_back(cLine(p1,p2));

    mDrawingLine = false;
    update();
}

void 
PartitionGameWindow::paintEvent(QPaintEvent* event){
    QPainter pt(this);
    pt.drawText(10,10,QString().sprintf("%d x %d balls = %d speed = %f",mGameWidth,mGameHeight,mNumBalls,mSpeed));
    for(size_t i = 0;i<mLines.size();++i){
        pt.drawLine(toQLine(mLines[i]));
    }
    if(mDrawingLine) pt.drawLine(toQLine(mCurrentLine));
}
