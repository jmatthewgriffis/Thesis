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
    void update(ofVec2f _pos, float _playerHeight, bool _allowControl);
    void draw();
    
    ofImage trebleClef, bassClef;
    
    ofVec2f pos, posPlayer, pointRear, pointFront;
    
    float angle, angleVel, rotPoint, anglePrev;
    bool bTiltUpward, bTiltDownward;
    bool onStream;
    bool clockwise;
    
    float fImgHeightTreble, fImgHeightBass, rotOffset;
};