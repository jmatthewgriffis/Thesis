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
    
    ofVec2f pos;
    ofVec2f vel;
    bool allowMove, moveU, moveL, moveD, moveR;
    bool record;
};