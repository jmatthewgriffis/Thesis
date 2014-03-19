//
//  Player.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"
#include "Object.h"

#define CAPACITY 9

class Player {
public:
    Player();
    
    void setup( int _iScaler, bool _bUsingController, ofVec2f _pos );
    void update( int _gameState );
    void draw( ofTrueTypeFont _font, vector< Object > _recordedList );
    
    void applyForce( ofVec2f _force );
    void fPressingRecord();
    void fPressingReplay();
    void fActing();
    
    ofVec2f pos, vel, acc, actPos;
    float wide, tall;
    float maxVel, jumpVel;
    bool allowMove, allowJump, up, left, down, right;
    bool onSurface;
    bool record, replay, bIsRecording, bIsReplaying, bAllowRecord, bAllowReplay, bIsActing, bIsEmpty, bIsFull, bUsingController;
    int iScaler, gameState, capacity;
    float radius, angle, angleVel;
    float fHealth, fHealthMax, fHealthLossSpeed;
    int tmpAngle;
    
    ofImage headphones, hand, appendage, appendage_mirrored, hat; // For some baffling reason the image is not moving when it should.
};