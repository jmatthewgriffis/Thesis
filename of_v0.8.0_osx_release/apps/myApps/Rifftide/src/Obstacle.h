//
//  Obstacle.h
//  Prototype_4_cam_and_tutorial
//
//  Created by J. Matthew Griffis on 11/18/13.
//
//

#pragma once

#include "ofMain.h"

class Obstacle {
public:
    Obstacle();
    
    void setup( ofVec2f _pos, float _wide, float _tall, bool fromBottom );
    void draw();
    
    ofVec2f pos;
    float wide, tall;
};