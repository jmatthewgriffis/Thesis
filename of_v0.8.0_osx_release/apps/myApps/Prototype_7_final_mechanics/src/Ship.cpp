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
    angle = 0;
    angleVel = 2;
    rotPoint = 0;
}

//--------------------------------------------------------------
void Ship::update(ofVec2f _pos, float _playerHeight) {
    
    pos = _pos;
    fImgHeightTreble = _playerHeight * 2.25;
    fImgHeightBass = fImgHeightTreble / 2;
    
    while (angle > 360) {
        angle -= 360;
    }
    while (angle < 0) {
        angle += 360;
    }
    
    if (bTiltUpward) {
        angle -= angleVel;
    }
    if (bTiltDownward) {
        angle += angleVel;
    }
}

//--------------------------------------------------------------
void Ship::draw() {
    
    ofPushMatrix();{
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        float rotOffset;
        rotOffset = fImgHeightBass * rotPoint;
        
        ofTranslate(pos.x + rotOffset, pos.y);
        ofRotate(angle);
        
        ofPushMatrix();{
            
            ofTranslate(-rotOffset, 0);
            
            ofPushMatrix();{
                //ofTranslate(pos);
                ofRotate(90);
                trebleClef.draw(0, 0, (fImgHeightTreble * trebleClef.getWidth() / trebleClef.getHeight()), fImgHeightTreble);
            }ofPopMatrix();
            
            ofPushMatrix();{
                ofTranslate(- fImgHeightTreble * 0.5, 0);
                ofRotate(135);
                bassClef.draw(0, 0, (fImgHeightBass * bassClef.getWidth() / bassClef.getHeight()), fImgHeightBass);
            }ofPopMatrix();
        }ofPopMatrix();
        
        ofSetColor(255,0,0);
        ofCircle(0,0,5);
    }ofPopMatrix();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

//--------------------------------------------------------------
