#include "Ball.h"


cBall::cBall(void):mPosition(0,0),mSpeed(0,0),mType(0),mRadius(1){

}


cBall::cBall(const point2d_t& p,const point2d_t& s, int t,number_t r) :mPosition(p),mSpeed(s),mType(t),mRadius(r){

}

point2d_t 
cBall::position() const{
    return mPosition;
}

point2d_t 
cBall::speed() const{
    return mSpeed;
}

int 
cBall::type() const{
    return mType;
}

number_t 
cBall::radius() const{
    return mRadius;
}

void 
cBall::setPosition(point2d_t val){
    mPosition = val;
}

void 
cBall::setSpeed(point2d_t val){
    mSpeed = val;
}

void 
cBall::setType(int val){
    mType = val;
}

void 
cBall::setRadius(number_t val){
    mRadius = val;
}

void 
cBall::step(number_t ts){
    mPosition += mSpeed*ts;
}

bool 
cBall::collide(const tLineList& lines,const tBallList& balls){
    tPointList normals;
    point2d_t p;
    
    for(tLineList::const_iterator it = lines.begin();it!=lines.end();++it){
        if(distance(*it,mPosition,&p)<mRadius) normals.push_back(normalized(mPosition-p));
    }
    
    for(tBallList::const_iterator it = balls.begin();it!=balls.end();++it){
        if(&(*it)==this) continue;
        if(distance(it->mPosition,mPosition)<mRadius+it->mRadius) normals.push_back(normalized(mPosition-it->mPosition));
    }
    if(normals.empty()) return false;

    point2d_t col_normal(0,0);
    for(tPointList::const_iterator it = normals.begin();it!=normals.end();++it){
        if(dot(*it,mSpeed)<0.0) col_normal += *it;
    }
    col_normal = normalized(col_normal);

    mSpeed -= 2.0*col_normal*dot(mSpeed,col_normal);

    return true;
}