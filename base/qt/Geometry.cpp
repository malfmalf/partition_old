#include "Geometry.h"
// #include <QtCore>
#include <assert.h>

const number_t default_comparison_epsilon = 1e-6;

bool 
numcmp(number_t n1,number_t n2,number_t epsilon){
    return fabs(n1-n2)<epsilon;
}

bool 
numcmp(number_t n1,number_t n2){
    return numcmp(n1,n2,default_comparison_epsilon);
}

bool 
between(number_t n,number_t from,number_t to){
    return numcmp(n,from) || numcmp(n,to) || (n>from && n<to) || (n>to && n<from);
}

bool 
between(number_t n,number_t from,number_t to,number_t epsilon){
    return between(n,from,to,default_comparison_epsilon);
}

number_t 
length(const point2d_t& p){
    return sqrt(lengthSquared(p));
}

number_t 
lengthSquared(const point2d_t& p){
    return p.x()*p.x() + p.y()*p.y();
}

point2d_t 
dir(const point2d_t& from,const point2d_t& to){
    return to - from;
}

point2d_t 
normalized(const point2d_t& p){
    number_t l = lengthSquared(p);
    if(numcmp(l,0.0)) return p;
    return p / sqrt(l);
}

point2d_t 
orthogonal(const point2d_t& p){
    return point2d_t(p.y(),-p.x());
}

point2d_t 
rotated(const point2d_t& p,number_t r){
    QTransform tr;
    tr.rotate(r);
    return tr.map(p);
}

point2d_t 
translated(const point2d_t& p,const point2d_t& t){
    QTransform tr;
    tr.translate(t.x(),t.y());
    return tr.map(p);
}

point2d_t 
transformed(const point2d_t& p,const point2d_t& t,number_t r){
    QTransform tr;
    tr.translate(t.x(),t.y());
    tr.rotate(r);
    return tr.map(p);
}

number_t 
cross(const point2d_t& p1,const point2d_t& p2){
    return p1.x()*p2.y() - p1.y()*p2.x();
}

number_t 
dot(const point2d_t& p1,const point2d_t& p2){
    return p1.x()*p2.x() + p1.y()*p2.y();
}

number_t 
distanceSquared(const point2d_t& p1,const point2d_t& p2){
    return lengthSquared(p2-p1);
}

number_t 
distance(const point2d_t& p1,const point2d_t& p2){
    return length(p2-p1);
}

number_t 
distance(const cLine& l,const point2d_t& p,point2d_t* closest /*= NULL*/){
    number_t lproj = dot(normalized(l.dir()),dir(l.p1(),p)/length(l.dir()));
    point2d_t c;
    if     (lproj<0.0)    c = l.p1();
    else if(lproj>1.0)    c = l.p2();
    else                  c = l.p1() + l.dir() * lproj;
    if(closest) *closest = c;
    return distance(c,p);
}

number_t 
project(const cLine& line,const point2d_t& point,point2d_t* result /*= NULL*/){
    number_t p = dot(line.dir(),point - line.p1())/length(line.dir());
    if(result) *result = line.p1() + line.dir()*p;
    return p;
}

tIsectResult 
isect_paralleldirs(const cLine& l1,const cLine& l2,point2d_t* result /*= NULL*/){
    number_t c = cross(l1.dir(),l1.p1()-l2.p1());

    //totalmente paralelas
    if(numcmp(c,0.0)) return EMPTY;
    
    //en la misma linea
    //cuando hablo de linea quiero decir linea infinita , los cLine son segmentos (tengo que cambiar el nombre)
    number_t l2p1 = project(l1,l2.p1());
    number_t l2p2 = project(l1,l2.p2());
    
    //misma linea , separadas
    if(l2p1>1.0 && l2p2>1.0) return EMPTY;
    if(l2p1<0.0 && l2p2<0.0) return EMPTY;

    //misma linea , son iguales
    if(numcmp(l2p1,0.0) && numcmp(l2p2,1.0)) return MULTIPLE_SAME;
    if(numcmp(l2p2,0.0) && numcmp(l2p1,1.0)) return MULTIPLE_SAME;

    //misma linea, una dentro de otra
    if(between(l2p1,0.0,1.0) && between(l2p2,0.0,1.0)) return MULTIPLE_L2_CONTAINED;
    if(l2p1<0.0 && l2p2>1.0) return MULTIPLE_L1_CONTAINED;
    if(l2p2<0.0 && l2p1>1.0) return MULTIPLE_L1_CONTAINED;

    //coincide un punto
    if(numcmp(l2p1,0.0) || numcmp(l2p1,1.0)){
        if(result) *result = l2.p1();
        return POINT;
    }
    if(numcmp(l2p2,0.0) || numcmp(l2p2,1.0)){
        if(result) *result = l2.p2();
        return POINT;
    }
    //multiple normal , miro cual de l2 esta dentro y lo devuelvo
    if(between(l2p1,0.0,1.0)){
        if(result) *result = l2.p1();
        return MULTIPLE;
    }
    else{
        if(result) *result = l2.p2();
        return MULTIPLE;
    }
    //ESTO ERA PARA INTENTAR DEVOLVER UN PUNTO CON SENTIDO EN ALGUNOS CASOS : YAGNI
//     //los p1 coinciden
//     if(numcmp(l2p1,0.0)){
//         if(l2p2<0.0){
//             if(result) *result = l1.p1();
//             return POINT;
//         }
//         if(l2p2<1.0){
//             if(result) *result = l2.p2();
//             return MULTIPLE_L2_CONTAINED
//         }
//         else{
//             if(result) *result = l1.p2();
//             return MULTIPLE_L1_CONTAINED
//         }
//     }
//     //los p2 coinciden
//     if(numcmp(l2p2,1.0)){
//         if(l2p1>1.0){
//             if(result) *result = l1.p2();
//             return POINT;
//         }
//         if(l2p1>0.0){
//             if(result) *result = l2.p1();
//             return MULTIPLE_L2_CONTAINED
//         }
//         else{
//             if(result) *result = l1.p1();
//             return MULTIPLE_L1_CONTAINED
//         }
//     }
//     //l2p1 y l1p2 coinciden
//     if(numcmp(l2p1,1.0)){
//         if(l2p2>1.0){
//             if(result) *result = l1.p2();
//             return POINT;
//         }
//         if(l2p2>0.0){
//             if(result) *result = l2.p2();
//             return MULTIPLE_L2_CONTAINED
//         }
//         else{
//             if(result) *result = l1.p1();
//             return MULTIPLE_L1_CONTAINED
//         }
//     }
//     //l2p2 y l1p1 coinciden
//     if(numcmp(l2p2,0.0)){
//         if(l2p1<0.0){
//             if(result) *result = l1.p1();
//             return POINT;
//         }
//         if(l2p1<0.0){
//             if(result) *result = l2.p1();
//             return MULTIPLE_L2_CONTAINED
//         }
//         else{
//             if(result) *result = l1.p2();
//             return MULTIPLE_L1_CONTAINED
//         }
//     }
}

tIsectResult 
isect(const cLine& l1,const cLine& l2,point2d_t* result /*= NULL*/){
    number_t t,cd;
    if(numcmp(length(l1.dir()),0.0) || numcmp(length(l2.dir()),0.0)) return INVALID;
    cd = cross(l1.dir(),l2.dir());
    if(numcmp(cd,0.0)){//paralelas
        return isect_paralleldirs(l1,l2,result);
    }

    point2d_t r = l1.p1() - l2.p1();

    t = cross(l2.dir(),r) /cd;
    if(t<0.0 || t>1.0) return EMPTY;

    r.setX(-r.x());
    r.setY(-r.y());
    cd = -cd;

    t = cross(l1.dir(),r)/cd;
    if(t<0.0 || t>1.0) return EMPTY;

    if(result) *result = l2.p1() + l2.dir()*t;

    return POINT;
}

tIsectResult 
rayisect_paralleldirs(const cLine& ray,const cLine& l,point2d_t* result /*= NULL*/){
    number_t c = cross(ray.dir(),ray.p1()-l.p1());

    //totalmente paralelas
    if(numcmp(c,0.0)) return EMPTY;
    
    //en la misma linea
    //cuando hablo de linea quiero decir linea infinita , los cLine son segmentos (tengo que cambiar el nombre)
    number_t lp1 = project(ray,l.p1());
    number_t lp2 = project(ray,l.p2());
    
    //misma linea , separadas
    if(lp1<0.0 && lp2<0.0) return EMPTY;

    //misma linea , l2 dentro
    if(lp1>0.0 && lp2>0.0){
        if(result) *result = lp1<lp2 ? l.p1():l.p2();
        return MULTIPLE_L2_CONTAINED;
    }

    //el otro caso , tampoco es que me itnerese mucho
    return MULTIPLE;
}

// from : http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrRay2Segment2.cpp
// The intersection of two lines is a solution to P0+s0*D0 = P1+s1*D1.
// Rewrite this as s0*D0 - s1*D1 = P1 - P0 = Q.  If D0.Dot(Perp(D1)) = 0,
// the lines are parallel.  Additionally, if Q.Dot(Perp(D1)) = 0, the
// lines are the same.  If D0.Dot(Perp(D1)) is not zero, then
//   s0 = Q.Dot(Perp(D1))/D0.Dot(Perp(D1))
// produces the point of intersection.  Also,
//   s1 = Q.Dot(Perp(D0))/D0.Dot(Perp(D1))

tIsectResult 
ray_isect(const cLine& ray,const cLine& l,point2d_t* result /*= NULL*/){
    number_t t,cd;
    if(numcmp(length(ray.dir()),0.0) || numcmp(length(l.dir()),0.0)) return INVALID;
    cd = cross(ray.dir(),l.dir());
    if(numcmp(cd,0.0)){//paralelas
        return rayisect_paralleldirs(ray,l,result);
    }

    point2d_t r = ray.p1() - l.p1();

    t = cross(l.dir(),r) /cd;
    if(t<0.0) return EMPTY;//por que es t dentro del rayo

    r.setX(-r.x());
    r.setY(-r.y());
    cd = -cd;

    t = cross(ray.dir(),r)/cd;
    if(t<0.0 || t>1.0) return EMPTY;

    if(result) *result = l.p1() + l.dir()*t;

    return POINT;
}

cLine::cLine(void) : mP1(0.0,0.0),mP2(0.0,0.0){

}


cLine::cLine(const point2d_t& p1,const point2d_t& p2) : mP1(p1),mP2(p2){

}

void 
cLine::set(const point2d_t& p1,const point2d_t& p2){
    mP1 = p1;
    mP2 = p2;
}

void 
cLine::setP1(const point2d_t& p1){
    mP1 = p1;
}

void 
cLine::setP2(const point2d_t& p2){
    mP2 = p2;
}

const point2d_t& 
cLine::p1(void) const{
    return mP1;
}

const point2d_t& 
cLine::p2(void) const{
    return mP2;
}

point2d_t
cLine::dir(void) const{
    return mP2-mP1;
}

cLine 
cLine::fromPointAndDir(const point2d_t& p,const point2d_t& d){
    return cLine(p,p+d);
}

int 
cLine::side(const point2d_t& p){
    point2d_t d = p - mP1;
    number_t s = cross(d,dir());
    if(numcmp(s,0.0)) return 0;
    else if(s<0.0) return -1;
    else return 1;
}

void 
cLine::rotateAroundCenter(number_t r){
    rotateAroundPoint(r,mP1 + dir() * 0.5);
}

void 
cLine::rotateAroundPoint(number_t r,const point2d_t& p){
    mP1 = rotated(mP1 - p , r);
    mP2 = rotated(mP2 - p , r);
}

void 
cLine::translate(const point2d_t& p){
    mP1 += p;
    mP2 += p;
}

point2d_t 
cLine::getPointInLine(number_t pos) const{
    return mP1 + dir()*pos;
}


bool 
ray_clip(const cLine& ray,const tLineList& clip_lines,point2d_t& point){
    number_t best_d1 = -1.0;
    for(size_t i = 0;i<clip_lines.size();++i){
        point2d_t p;
        tIsectResult r = ray_isect(ray,clip_lines[i],&p);
        if(r == POINT || r == MULTIPLE_L2_CONTAINED){
            number_t d = distanceSquared(ray.p1(),p);
            if(best_d1<0.0 || d<best_d1){
                point = p;
                best_d1 = d;
            }
        }
    }
    return best_d1>=0.0;
}