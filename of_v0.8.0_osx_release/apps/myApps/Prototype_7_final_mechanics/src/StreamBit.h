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
    void setup(float _noteWidth, float _noteHeight, ofVec2f _pos, ofVec2f _slope, float _destroyOffset, float _wide, int _angle);
    void update();
    void draw();
    
    ofVec2f pos, slope;
    float wide, tall, noteWidth, slurTall;
    float edgeToEdge, spaceBits, destroyOffset;
    int numBits;
    int angle;
    int opacity, opacityState, numStates;
    bool bDrawCollider;
    bool destroyMe;
    
    ofImage slur;
};