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
    slur.loadImage("images/slur.png");
}

//--------------------------------------------------------------
void StreamBit::setup(float _noteHeight, ofVec2f _pos) {
    pos = _pos;
    wide = _noteHeight;
    tall = slur.getHeight() * wide / slur.getWidth();
    destroyMe = false;
}

//--------------------------------------------------------------
void StreamBit::update(float _angle) {
    angle = _angle;
}

//--------------------------------------------------------------
void StreamBit::draw() {
    ofPushMatrix();{
        ofTranslate(pos);
        ofRotate(angle);
        ofSetColor(255, 255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        //slur.draw(0, 0, wide, tall);
        
        ofFill();
        ofSetColor(255,0,0, 255);
        ofCircle(0, 0, 5);
        //ofSetColor(0, 255);
        //ofSetLineWidth(3);
        //ofLine(0,0,0,-10);
        //ofCircle(0,-10, 2);
    }ofPopMatrix();
}

//--------------------------------------------------------------