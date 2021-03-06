#pragma once
#include "definitions.h"
#include <QtGui>

#include "Geometry.h"
#include "Ball.h"

class PartitionGameWindow : public QWidget {
    Q_OBJECT
public:
    PartitionGameWindow(QWidget* parent = NULL);
public slots:
    void step(void);
    void reset(void);
    void setNumBalls(int b);
    void setSpeed(double s);
    void setGameWidth(int w);
    void setGameHeight(int h);
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void paintEvent(QPaintEvent* event);

    void paintCurrentLine(QPainter &pt);
    void paintLines(QPainter &pt);
    void paintAreas(QPainter &pt);
    void paintBalls(QPainter &pt);
    bool clipLine(cLine& l);
private:
    QTimer* mTimer;
    int mNumBalls;
    double mSpeed;
    int mGameWidth;
    int mGameHeight;
    cLine mCurrentLine;
    bool mDrawingLine;

    typedef std::list<cPolygon> tPolygonList;
    tPolygonList mPolygons;
    tLineList mLines;
    tBallList    mBalls;

};
