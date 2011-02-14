#include "Game.h"
#include "Utils.h"

cGame::cGame(int w, int h) : mGameWidth(w),mGameHeight(h),mTotalArea(w*h){
    reset(1,10.0);
}

void 
cGame::reset(int numBalls,number_t speed){
    mNumBalls = numBalls;
    mSpeed    = speed;
    mBallSize = 6;
    mLines.clear();
    mBalls.clear();
    mPolygons.clear();
    mPolygonScores.clear();


    point2d_t center(gameWidth()/2,gameHeight()/2);
    point2d_t top_left     = center + point2d_t(-gameWidth()/2+2 , -gameHeight()/2+2);
    point2d_t top_right    = center + point2d_t( gameWidth()/2-2 , -gameHeight()/2+2);
    point2d_t bottom_right = center + point2d_t( gameWidth()/2-2 ,  gameHeight()/2-2);
    point2d_t bottom_left  = center + point2d_t(-gameWidth()/2+2 ,  gameHeight()/2-2);

    mLines.push_back(cLine(top_left     , top_right));
    mLines.push_back(cLine(top_right    , bottom_right));
    mLines.push_back(cLine(bottom_right , bottom_left));
    mLines.push_back(cLine(bottom_left  , top_left));
    
    cPolygon poly;
    poly.addPoint(top_left    );
    poly.addPoint(top_right   );
    poly.addPoint(bottom_right);
    poly.addPoint(bottom_left );
    mPolygons.push_back(poly);


    mBalls.clear();
    for(size_t i = 0;i<mNumBalls;++i){
        addRandomBall();
    }

    mScore = 0.0;
    mLinesLeft = 3;

}

cGame::GameResult 
cGame::step(number_t ts){
    for(tBallList::iterator it = mBalls.begin();it!=mBalls.end();++it){
        it->collide(mLines,mBalls);
    }
    for(tBallList::iterator it = mBalls.begin();it!=mBalls.end();++it){
        it->step(ts);
    }
    mScore = calcPolygonScores();
    if(mLinesLeft<1) return LOST_GAME;
    for(size_t i = 0;i<mPolygonScores.size();++i){
        if(mPolygonScores[i].numballs>1) return RUNNING;
    }
    return WON_GAME;
}

int 
cGame::score(void) const{
    return mScore;
}

int 
cGame::linesLeft(void) const{
    return mLinesLeft;
}

int 
cGame::gameWidth(void) const{
    return mGameWidth;
}

int 
cGame::gameHeight(void) const{
    return mGameHeight;
}

int 
cGame::numBalls(void) const{
    return mNumBalls;
}

number_t 
cGame::speed(void) const{
    return mSpeed;
}

const tLineList& 
cGame::lines(void) const{
    return mLines;
}

const tBallList& 
cGame::balls(void) const{
    return mBalls;
}

const cGame::tPolygonScoreList& 
cGame::polygonScores(void) const{
    return mPolygonScores;
}


bool 
cGame::clipLine(cLine& l) const{
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

bool 
cGame::addLine(const cLine& l){
    cLine r(l);
    if(!clipLine(r)){
        --mLinesLeft;
        mMessage = "Don't clip";
        return false;
    }
    if(collidingWithBalls(r)){
        --mLinesLeft;
        mMessage = "Colliding with balls";
        return false;
    }

    point2d_t medio(r.p1()*0.5+r.p2()*0.5);
    cLine tl(medio,medio+l.dir());

    for(tPolygonList::iterator it = mPolygons.begin();it!=mPolygons.end();++it){
        if(it->isInside(medio)){
            cPolygon pl1,pl2;
            if(!it->bisect(tl,pl1,pl2)){
//                qDebug()<<"FALLO DE BISECCION!!!!"<<endl;
//                it->bisect(l,pl1,pl2);
                mMessage = "Bisection failure "+tostring(it->idip1)+" "+tostring(it->idip2);
                return false;
            }
            *it = pl1;
            mPolygons.push_back(pl2);
            break;
        }
    }
    mMessage.clear();
    mLines.push_back(r);
    return true;
}

int 
cGame::calcPolygonScores(void){
    mPolygonScores.clear();
    int total_score = 0;
    for(tPolygonList::iterator it = mPolygons.begin();it!=mPolygons.end();++it){
        PolygonScore sc;
        sc.area   = it->area()/mTotalArea;
        sc.center = it->center();
        for(tBallList::iterator bit = mBalls.begin();bit!=mBalls.end();++bit){
            if(it->isInside(bit->position())) ++sc.numballs;
        }
        sc.score = sc.numballs ? (mNumBalls-sc.numballs)*(sc.area*10) : -10;
        total_score += sc.score;
        mPolygonScores.push_back(sc);
    }
    return total_score;
}

void 
cGame::addRandomBall(void){
    point2d_t pos(rand_number(mBallSize,mGameWidth-mBallSize),rand_number(mBallSize,mGameHeight-mBallSize));
    point2d_t vel = normalized(point2d_t(rand_number(-1000,1000)/1000.0,rand_number(-1000,1000)/1000.0))*mSpeed;
    cBall ball(pos,vel,0,mBallSize);
    while(ball.collide(mLines,mBalls)){
        ball.setPosition(point2d_t(rand_number(0,mGameWidth),rand_number(0,mGameHeight)));
    }
    mBalls.push_back(ball);
}

bool 
cGame::collidingWithBalls(const cLine& l) const{
    for(size_t i = 0;i<mBalls.size();++i){
        double d  = distance(l,mBalls[i].position());
        if(d<mBalls[i].radius()) return true;
    }
    return false;
}

const std::string& 
cGame::message(void) const{
    return mMessage;
}