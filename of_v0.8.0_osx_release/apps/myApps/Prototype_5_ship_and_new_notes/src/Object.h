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
    
    void setup( vector< float > _staffPosList, string _whichNote, int _age );
    void update( ofVec2f _pos );
    void draw();
    
    string whichNote;
    void fLimitLife();
    float fReturnYPos( string _whichNote );
    void fAddNote();
    
    int age;
    
    ofVec2f pos, vel;
    float tall;
    float wide;
    float guideLineLength;
    ofColor c;
    int colorTimer;
    
    bool destroyMe;
    bool drawAttention, bIsRecorded;
    
    vector< Note > noteList;
    vector< float > staffPosList;
    
//    Note myNote;
    
    float vol; // Volume
};