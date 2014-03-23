//
//  Player.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"
#include "Ship.h"
#include "Object.h"

#define CAPACITY 9

class Player {
public:
    Player();
    
    void setup( int _iScaler, bool _bUsingController, ofVec2f _pos, vector< float > _staffPosList );
    void update( int _gameState, string _OnThisNote );
    void draw( ofTrueTypeFont _font, vector< Object > _recordedList );
    
    void applyForce( ofVec2f _force );
    void fPressingRecord();
    void fPressingReplay();
    void fActing();
    void fDrawRecordedList(vector< Object > _recordedList);
    void fDrawCapacity(vector< Object > _recordedList);
    void fDrawAction(ofTrueTypeFont _font);
    void fDrawHealth();
    void fDrawCharacter();
    
    vector< float > staffPosList;
    
    ofVec2f pos, vel, acc, actPos;
    float yPosLast, yPosDiff, yPosStaff;
    float wide, tall;
    float maxVel, jumpVel;
    float fHatSizer, fHatWidth, fHatHeight, fHatOffset, fHatOffsetDefault, fHatVel, fHatVelDefault, fHatQueuedForce;
    
    // Master booleans to switch features on and off.
    bool bModePlatformer, bModeSurf, bModeFlight;
    bool bIsOnlyOneRoom;
    bool bHasShip;
    bool bCanMakeNotes;
    
    bool allowMove, allowJump, up, left, down, right;
    bool onSurface;
    bool record, replay, bIsRecording, bIsReplaying, bAllowRecord, bAllowReplay, bIsActing, bIsEmpty, bIsFull, bUsingController;
    int iScaler, gameState, capacity;
    float radius, angle, angleVel;
    float fHealth, fHealthMax, fHealthLossSpeed;
    float fNoteOffsetH;
    int tmpAngle;
    
    ofImage headphones, hand, appendage, appendage_mirrored, hat; // For some baffling reason the image is not moving when it should.
    
    Ship myShip;
    
    Object myObject; // Use just for reference to its properties. Doesn't get drawn or updated.
};