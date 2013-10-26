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
    
    void setup( int _whichNote, float y );
    void update();
    void draw();
    
    int whichNote;
    
    void addNote();
    
    ofVec2f pos;
    float vel;
    float tall;
    float wide;
    float guideLineLength;
};