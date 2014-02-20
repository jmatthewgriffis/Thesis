//
//  Track.h
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/18/14.
//
//

#pragma once

#include "ofMain.h"

class Track {
public:
    Track();
    
    void setup( int _iScaler, float _fMeasureLength/*, void *_addObject( string _note, float _xPos, int _age )*/ );
    void update();
    void draw();
    
    int iScaler;
    
    float fMeasureLength;
};