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
    void update(ofVec2f _pos, float _playerHeight, bool _allowControl, int _jumpCounter);
    void draw();
    
    ofImage trebleClef, bassClef;
    
    ofVec2f pos, posPlayer, pointRear, pointFront;
    
    float angle, angleVel, rotPoint, anglePrev, jumpAngle;
    bool bTiltUpward, bTiltDownward;
    bool bSolid, onStream, onStreamPrev, makeBigSplash;
    bool clockwise;
    bool bHasExtraJump;
    
    float fImgHeightTreble, fImgHeightBass, rotOffset;
};