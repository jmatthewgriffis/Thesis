//
//  Note.h
//  Prototype_1_Killer_Riff
//
//  Created by J. Matthew Griffis on 10/2/13.
//
//

#pragma once
#include "ofMain.h"

class Note {
public:
    Note();
    
    void setup( int _whichNote, float y );
    void update();
    void draw();
    
    int whichNote;
    
    ofVec2f pos;
    float vel;
    float tall;
    float wide;
    float guideLineLength;
};