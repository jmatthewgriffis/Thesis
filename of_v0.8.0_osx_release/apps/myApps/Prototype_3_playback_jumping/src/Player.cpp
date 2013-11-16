//
//  Player.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Player.h"

Player::Player() {
    
    up = left = down = right = onSurface = record = false;
    allowMove = allowJump = true;
    wide = 50;
    tall = wide;
    xVel = 5;
    jumpVel = 10;
}

void Player::setup() {
    
    pos.set( 100, 100 );
    vel.set( 0 );
    acc.set( 0 );
}

void Player::applyForce( ofVec2f _force ) {
    
    acc += _force;
}

void Player::update() {
    
    // Movement
    if ( allowMove ) {
        if ( up && onSurface && allowJump ) {
            //            pos.y -= vel.y;
            onSurface = false;
            allowJump = false;
            // Jump! And prevent additional jumps.
            applyForce( ofVec2f( 0.0, -jumpVel ) );
        }
        if ( left ) {
            applyForce( ofVec2f( -xVel, 0.0 ) );
        }
        if ( down ) {
            //            pos.y += vel.y;
        }
        if ( right ) {
            applyForce( ofVec2f( xVel, 0.0 ) );
        }
        
        vel += acc;
        
        // Negate velocity on a surface.
        if ( onSurface ) {
            vel.y = 0;
        }
        
        pos += vel;
        
//        cout<<vel.x<<endl;
        
//        if ( onSurface ) {
            vel.x = 0;
//        }
        
        // Manage forces.
        float damping = 0.97;
        vel.y *= damping;
        vel.x *= damping / 2.0;
        acc.set( 0 );
        
    } // End "if allowMove"
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