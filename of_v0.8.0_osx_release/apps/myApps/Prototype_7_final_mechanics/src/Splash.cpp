//
//  Splash.cpp
//  Prototype_7_final_mechanics
//
//  Created by J. Matthew Griffis on 4/14/14.
//
//

#include "Splash.h"

//--------------------------------------------------------------
Splash::Splash() {
    slur.loadImage("images/slur.png"); // http://upload.wikimedia.org/wikipedia/commons/thumb/1/10/Musical_Slur.svg/300px-Musical_Slur.svg.png
}

//--------------------------------------------------------------
void Splash::setup(ofVec2f _pos) {
    
    pos1 = _pos;
    pos2 = _pos;
    vel1.set(0);
    vel2.set(0);
    acc.set(0);
    
    wide = 50;
    tall = slur.getHeight() * wide / slur.getWidth();
    angle1 = angle2 = 0;
    alpha = 255;
    destroyMe = false;
    
    float up = -2; // Initial upward vel.
    float reducer = 2;
    vel1.set(up / -reducer, up);
    vel2.set(up / reducer, up);
}

//--------------------------------------------------------------
void Splash::applyForce(ofVec2f _force) {
    acc += _force;
}

//--------------------------------------------------------------
void Splash::update() {
    
    applyForce(ofVec2f(0, 0.3));
    
    vel1 += acc;
    vel2 += acc;
    
    angle1 = ofRadToDeg(atan2(vel1.y, vel1.x)) - 90;
    angle2 = ofRadToDeg(atan2(vel2.y, vel2.x)) - 90;
    
    pos1 += vel1;
    pos2 += vel2;
    
    acc.set(0);
    if (alpha > 0) {
        alpha -= 5;
    } else {
        destroyMe = true;
    }
}

//--------------------------------------------------------------
void Splash::draw() {
    
    // Test
    /*ofSetColor(255,0,0);
    ofCircle(pos1, 5);
    ofSetColor(0,255,0);
    ofCircle(pos2, 5);*/
    
    ofSetColor(255, alpha);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();{
        ofPushMatrix();{
            ofTranslate(pos1);
            ofRotate(angle1);
            slur.draw(0, 0, wide, tall);
        }ofPopMatrix();
        ofPushMatrix();{
            ofTranslate(pos2);
            ofRotate(angle2);
            slur.draw(0, 0, wide, tall);
        }ofPopMatrix();
    }ofPopMatrix();
}