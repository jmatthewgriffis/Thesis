//
//  Ship.h
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 3/22/14.
//
//

#pragma once

#include "ofMain.h"

class Ship {
public:
    Ship();
    
    void setup();
    void update(ofVec2f _pos, float _playerHeight);
    void draw();
    
    ofImage trebleClef, bassClef;
    
    ofVec2f pos, store;
    
    int angle, angleVel, rotPoint, streamAngle;
    bool bTiltUpward, bTiltDownward;
    
    float fImgHeightTreble, fImgHeightBass;
};