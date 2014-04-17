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
    angle = anglePrev = jumpAngle = 0;
    angleVel = 2;
    rotPoint = 1;
    onStream = onStreamPrev = makeBigSplash = false;
    bSolid = clockwise = bHasExtraJump = true;
}

//--------------------------------------------------------------
void Ship::update(ofVec2f _pos, float _playerHeight, bool _allowControl, int _jumpCounter) {
    
    if (_jumpCounter < 2) {
        bHasExtraJump = true;
        jumpAngle--;
    } else {
        bHasExtraJump = false;
        jumpAngle = 0;
    }
    
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
    
    // Store the front and rear rotation points.
    pointRear.x = posPlayer.x - rotOffset * cos(ofDegToRad(359));
    pointRear.y = posPlayer.y - rotOffset * sin(ofDegToRad(359));
    /*pointRear.x = posPlayer.x - rotOffset * cos(ofDegToRad(angle));
     pointRear.y = posPlayer.y - rotOffset * sin(ofDegToRad(angle));*/
    pointFront.x = posPlayer.x - rotOffset * sin(ofDegToRad(360 - 0 - 90));
    pointFront.y = posPlayer.y - rotOffset * cos(ofDegToRad(360 - 0 - 90));
    /*pointFront.x = posPlayer.x - rotOffset * sin(ofDegToRad(360 - angle - 90));
     pointFront.y = posPlayer.y - rotOffset * cos(ofDegToRad(360 - angle - 90));*/
    
    
    if (onStream) {
        if (angle >= 180) {
            rotPoint = -1; // Back of ship.
        } else if (angle < 180) {
            rotPoint = 1; // Front of ship.
        }
    } else {
        rotPoint = 0;
        /*if (angle >= 180 && !clockwise) {
            rotPoint = -1; // Back of ship.
        } else if (angle < 180 && clockwise) {
            rotPoint = 1; // Front of ship.
        }*/
    }
    
    // Set the position based on the rotation point so we can rotate the ship correctly.
    if (rotPoint == -1) {
        pos = pointRear;
    } else if (rotPoint == 1) {
        pos = pointFront;
    } else {
        pos = posPlayer;
    }
    
    anglePrev = angle;
    
    if (_allowControl) {
        float airMultiplier;
        if (onStream) {
            airMultiplier = 1;
        } else {
            airMultiplier = 2;
        }
        if (bTiltUpward) {
            angle -= angleVel * airMultiplier;
        }
        if (bTiltDownward) {
            angle += angleVel * airMultiplier;
        }
    }
    
    bSolid = true;
    if (angle > 90 && angle < 270) {
        bSolid = false;
    }
    
    // Figure out the overall direction of rotation by switching only when crossing zero.
    if (anglePrev >= 0 && angle < 0) {
        clockwise = false;
    } else if (anglePrev <= 360 && angle > 360) {
        clockwise = true;
    }
    
    if (!onStreamPrev && onStream) {
        makeBigSplash = true;
    }
    
    onStreamPrev = onStream;
}

//--------------------------------------------------------------
void Ship::draw() {
    
    int alpha;
    if (bSolid) {
        alpha = 255;
    } else {
        alpha = 127;
    }
    ofSetColor(255, alpha);
    
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
                
                if (bHasExtraJump) {
                    ofPushMatrix();{
                        ofRotate(-135);
                        ofPushMatrix();{
                            ofTranslate(-6, 19);
                            
                            /*ofSetColor(255, alpha);
                             ofCircle(0, 0, 17);
                             ofSetColor(255, alpha); // yo yo
                             ofCircle(0, 0, 15);*/
                            float margin = fImgHeightBass / -4.8;
                            float length = margin - (fImgHeightBass / 9.6);
                            for (int i = 0; i < 8; i++) {
                                ofPushMatrix();{
                                    ofRotate(jumpAngle + i * 45);
                                    ofSetLineWidth(3);
                                    ofSetColor(0, alpha);
                                    ofLine(0, margin, 0, length);
                                    ofSetLineWidth(1);
                                    ofSetColor(255, alpha);
                                    ofLine(0, margin, 0, length);
                                }ofPopMatrix();
                            }
                        }ofPopMatrix();
                    }ofPopMatrix();
                }
            }ofPopMatrix();
            
        }ofPopMatrix();
        
        // Test where the pos / rotation point is.
        /*ofSetColor(255,0,0);
        ofCircle(0,0,5);*/
        
    }ofPopMatrix();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}

//--------------------------------------------------------------
