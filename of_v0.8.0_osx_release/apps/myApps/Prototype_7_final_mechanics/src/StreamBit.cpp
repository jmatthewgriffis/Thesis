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
    destroyMe = false;
}

//--------------------------------------------------------------
void StreamBit::draw(float _angle) {
    ofPushMatrix();{
        ofTranslate(pos);
        ofRotate(_angle);
        ofFill();
        ofSetColor(255,0,0, 255);
        ofCircle(0, 0, 5);
        ofLine(0,0,0,-10);
    }ofPopMatrix();
}

//--------------------------------------------------------------