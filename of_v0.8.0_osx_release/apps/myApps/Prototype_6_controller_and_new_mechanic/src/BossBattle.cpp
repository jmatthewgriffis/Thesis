//
//  BossBattle.cpp
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/18/14.
//
//

#include "BossBattle.h"

BossBattle::BossBattle() {
    
}

void BossBattle::addObject( string _note, float _xPos, int _age ) {
    
    stringList.push_back( _note );
    stringList.push_back( ofToString( _xPos ) );
    stringList.push_back( ofToString( _age ) );
}

vector< string > BossBattle::setup( int _iScaler, float _fMeasureLength ) {
    
    stringList.clear();
    
    iScaler = _iScaler;
    fMeasureLength = _fMeasureLength;
    
    addObject( "a3", iScaler * 8, -1 );
    addObject( "c4", iScaler * 16, -1 );
    addObject( "e4", iScaler * 24, -1 );
    addObject( "a3", iScaler * 32, -1 );
    addObject( "a3", iScaler * 40, -1 );
    addObject( "d4", iScaler * 40, -1 );
    
    return stringList;
}