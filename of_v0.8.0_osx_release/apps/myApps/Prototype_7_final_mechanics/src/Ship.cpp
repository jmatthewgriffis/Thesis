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
    streamAngle = 0;
}

//--------------------------------------------------------------
void Ship::update(ofVec2f _pos, float _playerHeight) {
    
    pos = _pos;
    store = _pos;
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
        
        if (angle > 180) {
            rotPoint = -1;
        } else {
            rotPoint = 1;
        }
        
        float rotOffset;
        rotOffset = fImgHeightBass;
        if (rotPoint == -1) {
            pos.x = store.x - rotOffset * cos(ofDegToRad(angle));
            pos.y = store.y - rotOffset * sin(ofDegToRad(angle));
        } else if (rotPoint == 1) {
            pos.x = store.x - rotOffset * sin(ofDegToRad(360 - angle - 90));
            pos.y = store.y - rotOffset * cos(ofDegToRad(360 - angle - 90));
        } else {
            //pos = store;
        }
        ofTranslate(pos);
        ofRotate(angle);
        
        ofPushMatrix();{
            
            ofTranslate(-rotOffset * rotPoint, 0);
            
            ofPushMatrix();{
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
