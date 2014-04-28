//
//  Obstacle.cpp
//  Prototype_4_cam_and_tutorial
//
//  Created by J. Matthew Griffis on 11/18/13.
//
//

#include "Obstacle.h"

Obstacle::Obstacle() {
    
}

void Obstacle::setup( ofVec2f _pos, float _wide, float _tall, bool fromBottom ) {
    
    wide = _wide;
    tall = _tall;
    if ( fromBottom ) {
        pos.set( _pos.x, _pos.y - _tall );
    } else {
        pos.set( _pos );
    }
}

void Obstacle::draw() {
    
    ofSetRectMode( OF_RECTMODE_CORNER );
    
    ofSetColor( 0 );
    
    ofNoFill();
    ofRect( pos, wide, tall );
    ofFill();
}