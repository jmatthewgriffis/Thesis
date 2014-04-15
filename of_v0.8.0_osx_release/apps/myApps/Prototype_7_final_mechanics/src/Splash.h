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
    
    void setup(ofVec2f _pos);
    void update();
    void draw();
    void applyForce(ofVec2f _force);
    
    ofImage slur;
    
    ofVec2f pos1, pos2, vel, acc;
    
    float wide, tall, angle;
    float timer, limitTimer, limitAng;
};