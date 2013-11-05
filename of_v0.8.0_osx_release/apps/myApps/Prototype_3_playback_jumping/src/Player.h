//
//  Player.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"

class Player {
public:
    Player();
    
    void setup();
    void update();
    void draw();
    
    void applyForce( ofVec2f _force );
    
    ofVec2f pos, vel, acc;
    float wide, tall;
    bool allowMove, allowJump, moveU, moveL, moveD, moveR;
    bool record;
};