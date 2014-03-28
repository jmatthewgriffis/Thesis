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
    /*wide = _noteHeight;
    tall = slur.getHeight() * wide / slur.getWidth();*/
    wide = _wide;
    noteWidth = _noteWidth;
    tall = _noteHeight;
    slurTall = slur.getHeight() * tall / slur.getWidth();
    angle = _angle;
    opacityState = 0;
    opacity = 255;
    destroyMe = false;
    
    while (angle > 360){
        angle -= 360;
    }
    while (angle < 0){
        angle += 360;
    }
    
    /*
     The stream consists of a small element drawn repeatedly to fill the space between notes. But if there are too many, the program slows down. Optimizing for speed AND aesthetics requires that the minimum number of streamBits be drawn without losing too much of the stream. So, this function uses a series of calculations to figure out the exact space between the notes and fill it with evenly-spaced streamBits. Since the notes are often at different elevations and not circular, and it's important to draw from the edge only (not underneath or on top of the note), this gets complicated.
     */
    
    edgeToEdge = wide - _noteWidth;
    // Calculate how many elements can fit in between the notes.
    numBits = int(edgeToEdge / slurTall) * 0.67;
    spaceBits = (edgeToEdge - numBits * slurTall) / (numBits + 1);
}

//--------------------------------------------------------------
void StreamBit::update(float _angle) {
    /*angle = _angle;
    
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
    }*/
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
        for (int j = 0; j < numBits; j++) {
            ofPushMatrix();{
                ofSetColor(255, 255);
                ofTranslate(-wide * 0.5 + noteWidth * 0.5 + spaceBits + slurTall * 0.5 + (slurTall + spaceBits) * j, 0);
                ofRotate(ofRadToDeg(-PI * 0.5));
                slur.draw(0, 0, tall, slurTall);
            }ofPopMatrix();
        }
    }ofPopMatrix();
}

//--------------------------------------------------------------