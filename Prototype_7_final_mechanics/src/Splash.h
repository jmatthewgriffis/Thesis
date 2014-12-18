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
    
    void setup(ofVec2f _pos, float _wide, float _force, bool _trail = false);
    void update();
    void draw();
    void applyForce(ofVec2f _force);
    
    vector <ofVec2f> posList1;
    vector <ofVec2f> posList2;
    vector <float> angleList1;
    vector <float> angleList2;
    
    ofImage slur;
    
    ofVec2f pos1, pos2, vel1, vel2, acc;
    
    float wide, tall, angle1, angle2, alpha, alphaVel;
    
    bool trail, destroyMe;
};