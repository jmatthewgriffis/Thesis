//
//  Player.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Player.h"

Player::Player() {
    
    moveU = moveL = moveD = moveR = record = false;
    allowMove = true;
    wide = 50;
    tall = wide;
}

void Player::setup() {
    
    pos.set( 100, 100 );
    vel.set( 5, 0 );
    acc.set( 0 );
}

void Player::applyForce( ofVec2f _force ) {
    
    acc += _force;
}

void Player::update() {
    
    // Gravity
    applyForce( ofVec2f( 0.0, 0.3 ) );
    
    vel += acc;
    pos.y += vel.y;
    
    // Movement
    if ( allowMove ) {
        if ( moveU ) {
//            pos.y -= vel.y;
        }
        if ( moveL ) {
            pos.x -= vel.x;
        }
        if ( moveD ) {
//            pos.y += vel.y;
        }
        if ( moveR ) {
            pos.x += vel.x;
        }
    }
    
    // Land on the ground.
    if ( pos.y >= ofGetHeight() - tall / 2.0 ) {
        pos.y = ofGetHeight() - tall / 2.0;
        // Negate velocity on the ground.
        if ( !moveU ) {
            vel.y = 0;
        }
    }
    
    cout<<vel.y<<endl;
    
    // Manage forces.
    float damping = 0.97;
    vel.y *= damping;
    acc.set( 0 );
}

void Player::draw() {
    
    ofSetColor( 0 );
    ofSetRectMode( OF_RECTMODE_CENTER );
    ofRect( pos, wide, tall );
    
    // Draw a recording box if called.
    if ( record ) {
        ofSetColor( 0, 255, 0 );
        ofRect( pos.x + 50, pos.y, 20, 20 );
    }
}