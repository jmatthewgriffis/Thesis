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
    StreamBit(ofVec2f _pos = ofVec2f(0,0));
    void draw(float _angle = 0);
    
    ofVec2f pos;
    float wide;
    bool destroyMe;
};