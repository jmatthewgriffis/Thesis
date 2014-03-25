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
void StreamBit::setup(float _noteHeight, ofVec2f _pos) {
    pos = _pos;
    wide = _noteHeight;
    tall = slur.getHeight() * wide / slur.getWidth();
    opacityState = 0;
    opacity = 255;
    destroyMe = false;
}

//--------------------------------------------------------------
void StreamBit::update(float _angle) {
    angle = _angle;
    
    if (opacityState < 0) {
        opacityState += numStates;
    }
    if (opacityState > numStates - 1) {
        opacityState -= numStates;
    }
    
    float base = 75;
    float modifier = 25;
    if (opacityState == 0) {
        opacity = base;
    } else if (opacityState == 1) {
        opacity = base += modifier;
    } else if (opacityState == 2) {
        opacity = base += modifier * 2;
    } else if (opacityState == 3) {
        opacity = base += modifier * 3;
    } else {
        opacity = 255;
    }
}

//--------------------------------------------------------------
void StreamBit::draw() {
    ofPushMatrix();{
        ofTranslate(pos);
        ofRotate(angle);
        
        ofSetColor(255, opacity);
        ofSetRectMode(OF_RECTMODE_CENTER);
        slur.draw(0, 0, wide, tall);
        
        /*ofFill();
        ofSetColor(255,0,0, 255);
        ofCircle(0, 0, 5);*/
        //ofSetColor(0, 255);
        //ofSetLineWidth(3);
        //ofLine(0,0,0,-10);
        //ofCircle(0,-10, 2);
    }ofPopMatrix();
}

//--------------------------------------------------------------