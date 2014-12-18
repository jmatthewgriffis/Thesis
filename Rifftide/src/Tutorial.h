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
    
    vector< string >  setup( int _iScaler, int _iThirdOfScreen, bool _bIsLefty );
    void draw( ofTrueTypeFont _font );
    void addObject( string _note, float _xPos, int _age );
    void fWriteControls( bool bIsLefty );
    
    vector< string > stringList;
    vector< Obstacle > obstacleList;
    
    int iScaler, iThirdOfScreen;
    
    // Store a bunch of strings to accomodate different controls.
    string sUp, sLeft, sDown, sRight, sAltUp, sAltLeft, sAltDown, sAltRight;
};