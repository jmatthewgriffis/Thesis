//
//  StreamBit.h
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 3/23/14.
//
//

#pragma once
#define numStates 5

#include "ofMain.h"

class StreamBit {
public:
    StreamBit();
    void setup(float _noteHeight = 0, ofVec2f _pos = ofVec2f(0,0), float _wide = 0, int _angle = 0);
    void update(float _angle);
    void draw();
    
    ofVec2f pos, slope;
    float wide, tall;
    int angle;
    int opacity, opacityState;
    bool destroyMe;
    
    ofImage slur;
};