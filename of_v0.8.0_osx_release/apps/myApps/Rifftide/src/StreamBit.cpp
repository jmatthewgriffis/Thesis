//
//  StreamBit.cpp
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 3/23/14.
//
//

#include "StreamBit.h"

//--------------------------------------------------------------
StreamBit::StreamBit() {
    slur.loadImage("images/slur.png"); // http://upload.wikimedia.org/wikipedia/commons/thumb/1/10/Musical_Slur.svg/300px-Musical_Slur.svg.png
}

//--------------------------------------------------------------
void StreamBit::setup(float _noteWidth, float _noteHeight, ofVec2f _pos, ofVec2f _slope, float _destroyOffset, float _wide, int _angle, int _whichStream) {
    pos = _pos;
    slope = _slope;
    destroyOffset = _destroyOffset;
    wide = _wide;
    noteWidth = _noteWidth;
    tall = _noteHeight;
    slurTall = slur.getHeight() * tall / slur.getWidth();
    angle = _angle;
    whichStream = _whichStream;
    numStates = 5;
    opacityState = numStates - 1;
    opacity = 255;
    bDrawCollider = false;
    destroyMe = false;
    
    while (angle > 360){
        angle -= 360;
    }
    while (angle < 0){
        angle += 360;
    }
    
    // Calculate how many visual elements can fit in between the notes.
    edgeToEdge = wide - _noteWidth;
    numBits = int((edgeToEdge / slurTall) * 0.67) * 0.5;
    spaceBits = (edgeToEdge - numBits * slurTall) / (numBits + 1);
}

//--------------------------------------------------------------
void StreamBit::update() {
    
    while (opacityState < 0) {
        opacityState += numStates;
    }
    while (opacityState > numStates - 1) {
        opacityState -= numStates;
    }
}

//--------------------------------------------------------------
void StreamBit::draw(bool _onStream, int _playerStream) {
    ofPushMatrix();{
        ofTranslate(pos);
        ofRotate(angle);
        
        /*
         // Test circle.
         ofFill();
         ofSetColor(255,0,0, 255);
         ofCircle(0, 0, 5);
         //ofSetColor(0, 255);
         //ofSetLineWidth(3);
         //ofLine(0,0,0,-10);
         //ofCircle(0,-10, 2);
         */
        
        
         // Draw the collider.
        if (bDrawCollider) {
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(0);
            ofRect(0, 0, wide, tall);
            ofSetColor(255);
            ofRect(0, 0, wide - 2, tall - 2);
        }
        
        
        // Draw the slur "wave."
        ofSetRectMode(OF_RECTMODE_CENTER);
        float base = 125;
        float modifier = 0;
        for (int i = 0; i < numBits; i++) {
            ofPushMatrix();{
                
                float myOpacityState = opacityState - i;
                
                while (myOpacityState < 0) {
                    myOpacityState += numStates;
                }
                while (myOpacityState > numStates - 1) {
                    myOpacityState -= numStates;
                }
                
                if (myOpacityState == 0) {
                    opacity = base;
                } else if (myOpacityState == 1) {
                    opacity = base += modifier;
                } else if (myOpacityState == 2) {
                    opacity = base += modifier * 2;
                } else if (myOpacityState == 3) {
                    opacity = base += modifier * 3;
                } else {
                    opacity = 255;
                }
                
                float alpha;
                if (!_onStream || (_onStream && whichStream == _playerStream)) {
                    alpha = opacity;
                } else {
                    alpha = ofMap(opacity, 0, 255, 0, 100);
                }
                
                ofSetColor(255, alpha);
                ofTranslate(-wide * 0.5 + noteWidth * 0.5 + spaceBits + slurTall * 0.5 + (slurTall + spaceBits) * i, 0);
                ofRotate(ofRadToDeg(-PI * 0.5));
                slur.draw(0, 0, tall * 2, slurTall * 2);
            }ofPopMatrix();
        }
        
    }ofPopMatrix();
}

//--------------------------------------------------------------