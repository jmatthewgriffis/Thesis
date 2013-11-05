//
//  Player.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Player.h"

Player::Player() {
    
    allowJump = moveU = moveL = moveD = moveR = record = false;
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
    // Come back to this. Use it to fake analog sensitivity with jump height proportional to how long the button is held. Gravity only applies sometimes.
//    if ( !moveU ) {
        applyForce( ofVec2f( 0.0, 0.3 ) );
//    }
    
    // Movement
    if ( allowMove ) {
        if ( moveU && allowJump ) {
            //            pos.y -= vel.y;
            // Jump! And prevent additional jumps.
            applyForce( ofVec2f( 0.0, -10.0 ) );
            allowJump = false;
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
    
    vel += acc;
    pos.y += vel.y;
    
    // Land on the ground.
    if ( pos.y >= ofGetHeight() - tall / 2.0 ) {
        pos.y = ofGetHeight() - tall / 2.0;
        // Negate velocity on the ground.
        if ( !moveU ) {
            vel.y = 0;
            allowJump = true;
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