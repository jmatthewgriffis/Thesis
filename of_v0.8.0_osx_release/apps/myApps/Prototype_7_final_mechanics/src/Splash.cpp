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
    vel.set(0);
    acc.set(0);
    
    wide = 50;
    tall = slur.getHeight() * wide / slur.getWidth();
    
    timer = 0;
    limitTimer = 60;
    angle = 15;
    limitAng = 120;
    
    vel.y = -10;
}

//--------------------------------------------------------------
void Splash::applyForce(ofVec2f _force) {
    acc += _force;
}

//--------------------------------------------------------------
void Splash::update() {
    
    timer -= 0.5;
    
    if (abs(timer) > limitTimer && angle < limitAng) {
        //angle++;
    }
    
    applyForce(ofVec2f(0, 0.3));
    
    vel += acc;
    
    //pos1 = _pos + ofVec2f(vel.y * sin(ofDegToRad(-angle)), vel.y * cos(ofDegToRad(-angle)));
    //pos2 = _pos + ofVec2f(vel.y * sin(ofDegToRad(angle)), vel.y * cos(ofDegToRad(angle)));
    pos1 += vel;
    pos2 += vel;
    
    acc.set(0);
}

//--------------------------------------------------------------
void Splash::draw() {
    
    ofSetColor(0);
    //ofDrawBitmapString(ofToString(angle), pos1.x + 100, pos1.y);
    
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
            ofRotate(180 + angle);
            slur.draw(0, 0, wide, tall);
        }ofPopMatrix();
        ofPushMatrix();{
            ofTranslate(pos2);
            ofRotate(180 - angle);
            slur.draw(0, 0, wide, tall);
        }ofPopMatrix();
    }ofPopMatrix();
}