#pragma once
#include "definitions.h"
#include <vector>



typedef std::vector<point2d_t> tPointList;

extern const number_t default_comparison_epsilon;

bool numcmp(number_t n1,number_t n2);
bool numcmp(number_t n1,number_t n2,number_t epsilon);
bool between(number_t n,number_t from,number_t to);
bool between(number_t n,number_t from,number_t to,number_t epsilon);
number_t rand_number(void);

number_t  lengthSquared(const point2d_t& p);
number_t  length(const point2d_t& p);
point2d_t dir(const point2d_t& from,const point2d_t& to);
point2d_t normalized(const point2d_t& p);
point2d_t orthogonal(const point2d_t& p);
point2d_t rotated(const point2d_t& p,number_t r);
point2d_t translated(const point2d_t& p,const point2d_t& t);
point2d_t transformed(const point2d_t& p,const point2d_t& t,number_t r);
number_t  cross(const point2d_t& p1,const point2d_t& p2);
number_t  dot(const point2d_t& p1,const point2d_t& p2);

class cLine;

number_t distanceSquared(const point2d_t& p1,const point2d_t& p2);
number_t distance(const point2d_t& p1,const point2d_t& p2);
number_t distance(const cLine& l,const point2d_t& p,point2d_t* closest = NULL);
number_t project(const cLine& l,const point2d_t& p,point2d_t* point = NULL);
enum tIsectResult{
    INVALID,    //one of the lines is degenerate (a point)
    POINT,      //a point
    EMPTY,      //empty result , lines are parallel
    MULTIPLE,   //in the same line
    MULTIPLE_L1_CONTAINED, //in the same line and first one is contained in the second
    MULTIPLE_L2_CONTAINED, //in the same line and second one is contained in the first
    MULTIPLE_SAME
};
tIsectResult isect(const cLine& l1,const cLine& l2,point2d_t* result = NULL);
tIsectResult ray_isect(const cLine& ray,const cLine& l,point2d_t* result = NULL);

class cLine{
public:
    cLine(void);
    cLine(const point2d_t& p1,const point2d_t& p2);
    void set(const point2d_t& p1,const point2d_t& p2);
    void setP1(const point2d_t& p1);
    void setP2(const point2d_t& p2);
    const point2d_t& p1(void) const; 
    const point2d_t& p2(void) const; 
    point2d_t dir(void) const; 
    static cLine fromPointAndDir(const point2d_t& p,const point2d_t& d);
    int side(const point2d_t& p) const;
    void rotateAroundCenter(number_t r);
    void rotateAroundPoint(number_t r,const point2d_t& p);
    void translate(const point2d_t& p);
    point2d_t getPointInLine(number_t pos) const;//pos entre 0.0 y 1.0
private:
    point2d_t mP1;
    point2d_t mP2;
};

typedef std::vector<cLine> tLineList;

bool ray_clip(const cLine& ray,const tLineList& clip_lines,point2d_t& point);

class cPolygon{
public:
    cPolygon(void);
    cPolygon(const tPointList& pl);
    void clear(void);
    bool isValid(void) const;
    void addPoint(const point2d_t& p);
    int  numPoints(void) const;
    const tPointList& points(void) const;
    const point2d_t& operator[] (int i) const;
    bool isInside(const point2d_t& p) const;//valido solo para convex
    bool isect(const cLine& l,tPointList* results) const;
    bool clip(cLine& l) const;
    bool ray_clip(const cLine& l,point2d_t& point) const;
    bool bisect(const cLine& l,cPolygon& pl1,cPolygon& pl2);
private:
    tPointList mPoints;
    tLineList  mLines;
private:
    void _calclines(void);
};


