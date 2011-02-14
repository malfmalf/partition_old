#pragma once
#include "definitions.h"
#include "Geometry.h"
#include <vector>

class cBall;
typedef std::vector<cBall> tBallList;

class cBall{
public:
    cBall(void);
    cBall(const point2d_t& p,const point2d_t& s, int t,number_t r);
    point2d_t position() const;
    point2d_t speed() const;
    int       type() const;
    number_t  radius() const;

    void      setPosition(point2d_t val);
    void      setSpeed(point2d_t val);
    void      setType(int val);
    void      setRadius(number_t val);

    void      step(number_t ts);
    bool      collide(const tLineList& lines,const tBallList& balls);

private:
    point2d_t mPosition;
    point2d_t mSpeed;
    int       mType;
    number_t  mRadius;
};

