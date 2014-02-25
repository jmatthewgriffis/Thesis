//
//  BossBattle.h
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/18/14.
//
//

#pragma once

#include "ofMain.h"

class BossBattle {
public:
    BossBattle();
    
    vector< string > setup( int _iScaler, float _fMeasureLength );
    void addObject( string _note, float _xPos, int _age );
    
    vector< string > stringList;
    
    int iScaler;
    
    float fMeasureLength;
};