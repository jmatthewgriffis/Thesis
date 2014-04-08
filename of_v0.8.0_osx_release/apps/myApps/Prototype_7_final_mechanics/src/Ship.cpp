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
    
    posPlayer = _pos;
    pos = posPlayer;
    fImgHeightTreble = _playerHeight * 2.25;
    fImgHeightBass = fImgHeightTreble / 2;
    rotOffset = fImgHeightBass;
    
    while (angle > 360) {
        angle -= 360;
    }
    while (angle < 0) {
        angle += 360;
    }
    
    if (angle >= 180) {
        rotPoint = -1; // Back of ship.
    } else {
        rotPoint = 1; // Front of ship.
    }
    
    // Set the position based on the rotation point so we can rotate the ship correctly.
    if (rotPoint == -1) {
        pos.x = posPlayer.x - rotOffset * cos(ofDegToRad(angle));
        pos.y = posPlayer.y - rotOffset * sin(ofDegToRad(angle));
    } else if (rotPoint == 1) {
        pos.x = posPlayer.x - rotOffset * sin(ofDegToRad(360 - angle - 90));
        pos.y = posPlayer.y - rotOffset * cos(ofDegToRad(360 - angle - 90));
    } else {
        //pos = store;
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
        
        // Test where the pos / rotation point is.
        /*ofSetColor(255,0,0);
        ofCircle(0,0,5);*/
        
    }ofPopMatrix();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

//--------------------------------------------------------------
