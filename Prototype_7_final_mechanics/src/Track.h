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
    
    vector< string > setup( int _iScaler, float _fMeasureLength, int _gameState );
    void addObject( string _note, float _xPos, int _stream, int _age );
    void draw( ofTrueTypeFont _font );
    
    vector< string > stringList;
    
    int iScaler, gameState, iNumMeasures;
    
    float fMeasureLength;
};