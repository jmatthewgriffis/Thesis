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
void Splash::setup(ofVec2f _pos, float _wide, float _force, bool _trail) {
    
    pos1 = _pos;
    pos2 = _pos;
    vel1.set(0);
    vel2.set(0);
    acc.set(0);
    
    wide = _wide;
    tall = slur.getHeight() * wide / slur.getWidth();
    angle1 = angle2 = 0;
    float lightener = 0.5;
    alpha = 255 * lightener;
    alphaVel = (30 / abs(_force)) * lightener; // Inversely proportional.
    trail = _trail;
    destroyMe = false;
    
    float reducer = 4;
    vel1.set(_force / -reducer, _force);
    vel2.set(_force / reducer, _force);
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
    angleList1.push_back(angle1);
    angle2 = ofRadToDeg(atan2(vel2.y, vel2.x)) - 90;
    angleList2.push_back(angle2);
    
    pos1 += vel1;
    posList1.push_back(pos1);
    pos2 += vel2;
    posList2.push_back(pos2);
    
    acc.set(0);
    if (alpha > 0) {
        alpha -= alphaVel;
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
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    float wider = wide * 2;
    float taller = tall * 2;
    
    ofSetColor(255, alpha);
    ofPushMatrix();{
        ofPushMatrix();{
            ofTranslate(pos1);
            ofRotate(angle1);
            slur.draw(0, 0, wider, taller);
        }ofPopMatrix();
        ofPushMatrix();{
            ofTranslate(pos2);
            ofRotate(angle2);
            slur.draw(0, 0, wider, taller);
        }ofPopMatrix();
    }ofPopMatrix();
    
    if (trail) {
        float inc = 5;
        for (int i = 0; i < posList1.size(); i += inc) {
            float alphaTrail = alpha - inc - posList1.size() * inc + i * inc;
            if (alphaTrail < 0) {
                alphaTrail = 0;
            }
            ofSetColor(255, alphaTrail);
            ofPushMatrix();{
                ofTranslate(posList1[i]);
                ofRotate(angleList1[i]);
                slur.draw(0, 0, wider, taller);
            }ofPopMatrix();
            ofPushMatrix();{
                ofTranslate(posList2[i]);
                ofRotate(angleList2[i]);
                slur.draw(0, 0, wider, taller);
            }ofPopMatrix();
        }
    }
}