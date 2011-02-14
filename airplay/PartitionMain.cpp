/*
 * This file is part of the Airplay SDK Code Samples.
 *
 * Copyright (C) 2001-2010 Ideaworks3D Ltd.
 * All Rights Reserved.
 *
 * This source code is intended only as a supplement to Ideaworks Labs
 * Development Tools and/or on-line documentation.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include "s3e.h"
#include "IwUtil.h"
#include "Iw2D.h"
#include "IwUI.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Ball.h"
#include "Geometry.h"
#include "Game.h"

CIwSVec2 toscreen(const point2d_t& p){
    return CIwSVec2(int(p.x),int(p.y));
}

std::string gMessage;

void fillScreen(const cGame& game,cLine* drawing_line  =NULL){
    Iw2DSurfaceClear(0);
    Iw2DSetColour(0xffffffff);


    for(size_t i = 0;i<game.lines().size();++i){
        Iw2DDrawLine(toscreen(game.lines()[i].p1()),toscreen(game.lines()[i].p2()));
    }
    if(drawing_line){
        if(game.collidingWithBalls(*drawing_line)) Iw2DSetColour(0xff0000ff);
        else                               Iw2DSetColour(0xffff00ff);
        Iw2DDrawLine(toscreen(drawing_line->p1()),toscreen(drawing_line->p2()));
    }
    Iw2DSetColour(0xff00ff00);
    for(size_t i = 0;i<game.balls().size();++i){
         Iw2DDrawArc(toscreen(game.balls()[i].position()),toscreen(point2d_t(game.balls()[i].radius(),game.balls()[i].radius())),-IW_ANGLE_PI,IW_ANGLE_PI);
    }
    for(size_t i = 0;i<game.polygonScores().size();++i){
        const cGame::PolygonScore& sc = game.polygonScores()[i];
        char sc_text[100];
        sprintf(sc_text,"%d",sc.score);
        Iw2DDrawString(sc_text,toscreen(sc.center),CIwSVec2(30,30),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_TOP);
    }
    char text[300];
    sprintf(text,"l:%d , s:%d",game.linesLeft(),game.score());
    Iw2DDrawString(text,CIwSVec2(10,10),CIwSVec2(200,200),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_TOP);
    Iw2DDrawString(game.message().c_str(),CIwSVec2(10,20),CIwSVec2(200,200),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_TOP);
    Iw2DDrawString(gMessage.c_str(),CIwSVec2(10,30),CIwSVec2(300,600),IW_2D_FONT_ALIGN_LEFT,IW_2D_FONT_ALIGN_TOP);
    Iw2DSurfaceShow();

}


int main(){
    // Initialise
    Iw2DInit();

    IwResManagerInit();
    
    // Load the group containing the "arial14" font
    IwGetResManager()->LoadGroup("Iw2DStrings.group");
    
    // Prepare the iwgxfont resource for rendering using Iw2D
    CIw2DFont* font = Iw2DCreateFontResource("trebuchet");
    Iw2DSetFont(font);

    cGame game(s3eSurfaceGetInt(S3E_SURFACE_DEVICE_WIDTH),s3eSurfaceGetInt(S3E_SURFACE_DEVICE_HEIGHT));
    game.reset(15,40.0);
    int64 last_time = s3eTimerGetMs();
    bool drawing = false;
    point2d_t start_point;
    while (!s3eDeviceCheckQuitRequest())
    {
        int64 t = s3eTimerGetMs();
        int delta = t-last_time;
        last_time = t;

        if(game.step(delta/1000.0)!=cGame::RUNNING) break;


        s3eDeviceYield(0);
        s3eKeyboardUpdate();
        s3ePointerUpdate();
        point2d_t touch_point(s3ePointerGetX(),s3ePointerGetY());
        if(s3ePointerGetState(S3E_POINTER_BUTTON_SELECT)==S3E_POINTER_STATE_DOWN){
            if(!drawing){
                start_point = touch_point;
                drawing = true;
                fillScreen(game);
            }
            else{
                cLine line(start_point,touch_point);
                game.clipLine(line);
                fillScreen(game,&line);
            }
        }
        else{
            if(drawing){
                game.addLine(cLine(start_point,touch_point));
                drawing = false;
            }
            fillScreen(game);
        }

        if (s3eKeyboardAnyKey())
            break;
    }
    Iw2DTerminate();
    return 0;
}
