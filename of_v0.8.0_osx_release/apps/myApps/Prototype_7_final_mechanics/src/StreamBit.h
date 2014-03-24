//
//  StreamBit.h
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 3/23/14.
//
//

#pragma once

#include "ofMain.h"

class StreamBit {
public:
    StreamBit();
    void setup(float _noteHeight = 0, ofVec2f _pos = ofVec2f(0,0));
    void update(float _angle);
    void draw();
    
    ofVec2f pos;
    float wide, tall;
    float angle;
    bool destroyMe;
    
    ofImage slur;
};