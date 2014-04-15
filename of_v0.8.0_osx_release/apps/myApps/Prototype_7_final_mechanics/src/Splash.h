//
//  Splash.h
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 4/14/14.
//
//

#pragma once

#include "ofMain.h"

class Splash {
public:
    Splash();
    
    void draw(ofVec2f _pos);
    
    ofImage slur;
    
    ofVec2f pos, pos1, pos2;
    
    float wide, tall, angle1, angle2;
    float timer;
};