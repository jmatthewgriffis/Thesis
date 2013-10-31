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
}

void Player::setup() {
    
    pos.set( 100, 100 );
    vel.set( 5, 5 );
}

void Player::update() {
    
    // Movement
    if ( allowMove ) {
        if ( moveU ) {
            pos.y -= vel.y;
        }
        if ( moveL ) {
            pos.x -= vel.x;
        }
        if ( moveD ) {
            pos.y += vel.y;
        }
        if ( moveR ) {
            pos.x += vel.x;
        }
    }
}

void Player::draw() {
    
    ofSetColor( 0 );
    ofSetRectMode( OF_RECTMODE_CENTER );
    ofRect( pos, 50, 50 );
    
    // Draw a recording box if called.
    if ( record ) {
        ofSetColor( 0, 255, 0 );
        ofRect( pos.x + 50, pos.y, 20, 20 );
    }
}