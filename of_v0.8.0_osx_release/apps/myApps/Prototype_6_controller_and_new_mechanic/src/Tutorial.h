//
//  Tutorial.h
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/18/14.
//
//

#pragma once

#include "ofMain.h"
#include "Obstacle.h"

class Tutorial {
public:
    Tutorial();
    
    vector< string >  setup( int _iScaler, int _iThirdOfScreen );
    void update();
    void draw();
    void addObject( string _note, float _xPos, int _age );
    
    vector< string > stringList;
    vector< Obstacle > obstacleList;
    
    int iScaler, iThirdOfScreen;
};