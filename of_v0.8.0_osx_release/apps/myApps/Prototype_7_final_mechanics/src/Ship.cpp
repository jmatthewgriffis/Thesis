//
//  Ship.cpp
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 3/22/14.
//
//

#include "Ship.h"

//--------------------------------------------------------------
Ship::Ship() {
    
    trebleClef.loadImage( "images/clef_treble.png" ); // http://clipartist.info/SVG/CLIPARTIST.ORG/TREBLE/treble_clef_treble_clef-555px.png
    bassClef.loadImage( "images/clef_bass.png" ); // http://clipartist.info/RSS/openclipart.org/2011/April/15-Friday/bass_clef_bassclef-1979px.png
}

//--------------------------------------------------------------
void Ship::setup() {
    
}

//--------------------------------------------------------------
void Ship::update(ofVec2f _pos, float _playerHeight) {
    
    pos = _pos;
    fImgHeightTreble = _playerHeight * 2.25;
    fImgHeightBass = fImgHeightTreble / 2;
}

//--------------------------------------------------------------
void Ship::draw() {
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    ofPushMatrix();{
        ofTranslate(pos);
        ofRotate(90);
        trebleClef.draw(0, 0, (fImgHeightTreble * trebleClef.getWidth() / trebleClef.getHeight()), fImgHeightTreble);
    }ofPopMatrix();
    
    ofPushMatrix();{
        ofTranslate(pos.x - fImgHeightTreble * 0.5, pos.y);
        ofRotate(135);
        bassClef.draw(0, 0, (fImgHeightBass * bassClef.getWidth() / bassClef.getHeight()), fImgHeightBass);
    }ofPopMatrix();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

//--------------------------------------------------------------
