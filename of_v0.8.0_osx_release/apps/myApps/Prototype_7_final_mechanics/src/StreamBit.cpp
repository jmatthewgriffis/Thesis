//
//  StreamBit.cpp
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 3/23/14.
//
//

#include "StreamBit.h"

//--------------------------------------------------------------
StreamBit::StreamBit(ofVec2f _pos) {
    pos = _pos;
    wide = 5;
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
        ofFill();
        ofSetColor(0,0,255, 255);
        ofCircle(0, 0, wide);
        /*ofSetColor(0, 255);
        ofSetLineWidth(3);
        ofLine(0,0,0,-10);
        ofCircle(0,-10, 2);*/
    }ofPopMatrix();
}

//--------------------------------------------------------------