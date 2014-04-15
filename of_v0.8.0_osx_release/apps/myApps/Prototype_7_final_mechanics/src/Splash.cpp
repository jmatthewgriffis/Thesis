//
//  Splash.cpp
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 4/14/14.
//
//

#include "Splash.h"

Splash::Splash() {
    slur.loadImage("images/slur.png"); // http://upload.wikimedia.org/wikipedia/commons/thumb/1/10/Musical_Slur.svg/300px-Musical_Slur.svg.png
    wide = 50;
    tall = slur.getHeight() * wide / slur.getWidth();
    
    timer = 0;
    float angleOffset = 15;
    angle1 = -angleOffset;
    angle2 = angleOffset;
}

void Splash::draw(ofVec2f _pos) {
    pos = _pos;
    
    timer -= 0.5;
    
    pos1 = _pos + ofVec2f(timer * sin(ofDegToRad(angle1)), timer * cos(ofDegToRad(angle1)));
    pos2 = _pos + ofVec2f(timer * sin(ofDegToRad(angle2)), timer * cos(ofDegToRad(angle2)));
    
    // Test
    ofSetColor(255,0,0);
    ofCircle(pos1, 5);
    ofSetColor(0,255,0);
    ofCircle(pos2, 5);
    
    float posOffset = wide * 0.5;
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();{
        ofPushMatrix();{
            ofTranslate(pos1);
            ofRotate(180 + angle2);
            slur.draw(0, 0, wide, tall);
        }ofPopMatrix();
        ofPushMatrix();{
            ofTranslate(pos2);
            ofRotate(180 + angle1);
            slur.draw(0, 0, wide, tall);
        }ofPopMatrix();
    }ofPopMatrix();
}