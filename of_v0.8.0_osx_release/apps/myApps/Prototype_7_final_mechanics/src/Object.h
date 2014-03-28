//
//  Object.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"
#include "Note.h"

#define numPos 27

class Object {
public:
    Object();
    
    void setup( int _iScaler, vector< float > _staffPosList, string _whichNote, int _age );
    void update( int _gameState, ofVec2f _pos );
    void draw();
    
    string whichNote;
    void fLimitLife();
    float fReturnYPos( string _whichNote );
    void fAddNote();
    void fCleanup();
    
    int iScaler, gameState, age;
    
    ofVec2f pos, vel;
    float tall;
    float wide;
    float guideLineLength;
    ofColor c;
    int colorTimer, noteTimer;
    
    bool destroyMe;
    bool drawAttention, bIsRecorded, bIsTouched, bWasTouched, bIsPartOfStream, bHasFalloffLeft, bHasFalloffRight;
    
    vector< Note > noteList;
    vector< float > staffPosList;
    
//    Note myNote;
    
    float vol; // Volume
    int noteDuration;
};