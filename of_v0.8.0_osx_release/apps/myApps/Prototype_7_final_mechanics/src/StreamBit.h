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
    void setup(float _noteWidth, float _noteHeight, ofVec2f _pos, float _wide, int _angle);
    void update(float _angle);
    void draw();
    
    ofVec2f pos, slope;
    float wide, tall, noteWidth, slurTall;
    float edgeToEdge, spaceBits;
    int numBits;
    int angle;
    int opacity, opacityState;
    bool destroyMe;
    
    ofImage slur;
};