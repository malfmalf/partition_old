#pragma once
#include "definitions.h"
#include "Geometry.h"
#include "Ball.h"
#include <vector>


class cGame{
public:
    enum GameResult{
        WON_GAME,
        LOST_GAME,
        RUNNING
    };
    
    struct PolygonScore{
        PolygonScore(void) : score(0),numballs(0),area(0.0),center(0,0){;}
        int score;
        int numballs;
        number_t area;
        point2d_t center;
    };

    typedef std::vector<PolygonScore> tPolygonScoreList;

    cGame(int w, int h);
    void reset(int numBalls,number_t speed);
    GameResult step(number_t ts);
    int score(void) const;
    int linesLeft(void) const;
    int gameWidth(void) const;
    int gameHeight(void) const;
    int numBalls(void) const;
    number_t speed(void) const;
    const tLineList& lines(void) const;
    const tBallList& balls(void) const;

    const tPolygonScoreList& polygonScores(void) const;

    bool clipLine(cLine& l) const;
    bool addLine(const cLine& l);
    bool collidingWithBalls(const cLine& l) const;
    const std::string& message(void) const;

private:
    int mGameWidth;
    int mGameHeight;
    int mNumBalls;
    number_t mSpeed;
    tLineList mLines;
    tPolygonList mPolygons;
    tBallList mBalls;
    tPolygonScoreList mPolygonScores;
    int mScore;
    int mLinesLeft;
    int mBallSize;
    const int mTotalArea;
    std::string mMessage;
private:
    int calcPolygonScores(void);
    void addRandomBall(void);

};

