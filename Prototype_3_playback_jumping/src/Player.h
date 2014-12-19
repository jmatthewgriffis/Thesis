//
//  Player.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"

class Player {
public:
    Player();
    
    void setup();
    void update();
    void draw();
    
    void applyForce( ofVec2f _force );
    void fPressingRecord();
    void fPressingReplay();
    void fActing();
    
    ofVec2f pos, vel, acc, actPos;
    float wide, tall;
    float xVel, jumpVel;
    bool allowMove, allowJump, up, left, down, right;
    bool onSurface;
    bool record, replay, bIsRecording, bIsReplaying, bAllowRecord, bAllowReplay, bIsActing;
    float radius, angle, angleVel;
};