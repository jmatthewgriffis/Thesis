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

class Object {
public:
    Object();
    
    void setup( int _whichNote, float y, int _age );
    void update( ofVec2f _pos );
    void draw();
    
    int whichNote;
    void fLimitLife();
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
    
//    Note myNote;
    
    float vol; // Volume
};