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
void StreamBit::setup(float _noteWidth, float _noteHeight, ofVec2f _pos, float _wide, int _angle) {
    pos = _pos;
    slope.set(0);
    wide = _wide;
    noteWidth = _noteWidth;
    tall = _noteHeight;
    slurTall = slur.getHeight() * tall / slur.getWidth();
    angle = _angle;
    numStates = 5;
    opacityState = numStates - 1;
    opacity = 255;
    destroyMe = false;
    
    while (angle > 360){
        angle -= 360;
    }
    while (angle < 0){
        angle += 360;
    }
    
    // Calculate how many visual elements can fit in between the notes.
    edgeToEdge = wide - _noteWidth;
    numBits = int(edgeToEdge / slurTall) * 0.67;
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
void StreamBit::draw() {
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
        
        /*
        // Draw the collider.
        ofSetColor(0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        //ofRect(0, 0, wide, tall);
        */
        
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        // Draw the slur "wave."
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
                
                ofSetColor(255, opacity);
                ofTranslate(-wide * 0.5 + noteWidth * 0.5 + spaceBits + slurTall * 0.5 + (slurTall + spaceBits) * i, 0);
                ofRotate(ofRadToDeg(-PI * 0.5));
                slur.draw(0, 0, tall, slurTall);
            }ofPopMatrix();
        }
        
    }ofPopMatrix();
}

//--------------------------------------------------------------