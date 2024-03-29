//
//  Player.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Player.h"

Player::Player() {
    
    wide = 50;
    tall = wide;
    xVel = 5;
    jumpVel = 10;
    radius = 50;
    angleVel = 10;
}

void Player::setup() {
    
    up = left = down = right = onSurface = record = replay = bIsActing = bIsRecording = bIsReplaying = false;
    allowMove = allowJump = bAllowRecord = bAllowReplay = true;
    angle = 0;
    
    pos.set( 100, 100 );
    vel.set( 0 );
    acc.set( 0 );
    actPos.set( 0 );
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
        
    } // End "if allowMove"
    
    // Manage forces.
    float damping = 0.97;
    vel.y *= damping;
    vel.x *= damping / 2.0;
    acc.set( 0 );
    
    fPressingRecord();
    fPressingReplay();
    fActing();
}

void Player::draw() {
    
    ofSetColor( 0 );
    ofSetRectMode( OF_RECTMODE_CENTER );
    ofRect( pos, wide, tall );
    
    // Draw the action if called, orbiting around the player's pos.
    if ( bIsActing ) {
        if ( bIsRecording ) ofSetColor( 0, 255, 0 );
        else if ( bIsReplaying ) ofSetColor( 0, 0, 255 );
        ofCircle( actPos, 10 );
    }
}

void Player::applyForce( ofVec2f _force ) {
    
    acc += _force;
}

void Player::fPressingRecord() {
    
    // Player is pressing the record key.
    if ( record ) {
        // Make sure not already acting and/or holding the key down.
        if ( !bIsActing && bAllowRecord ) {
            // Trigger the action. Prevent additional calls while this one is going.
            angle = 225;
            bIsActing = true;
            bIsRecording = true;
            bAllowRecord = false;
        }
    } else if ( !bIsActing ) {
        // If not already pressing the key AND not acting, enable recording.
        bAllowRecord = true;
    }
}

void Player::fPressingReplay() {
    
    // Player is pressing the replay key.
    if ( replay ) {
        // Make sure not already acting and/or holding the key down.
        if ( !bIsActing && bAllowReplay ) {
            // Trigger the action. Prevent additional calls while this one is going.
            angle = 315;
            bIsActing = true;
            bIsReplaying = true;
            bAllowReplay = false;
        }
    } else if ( !bIsActing ) {
        // If not already pressing the key AND not acting, enable recording.
        bAllowReplay = true;
    }
}

void Player::fActing() {
    
    if ( bIsRecording ) {
        angle -= angleVel;
        if ( angle < -135 ) {
            bIsRecording = false;
        }
    } else if ( bIsReplaying ) {
        angle += angleVel;
        if ( angle > 675 ) {
            bIsReplaying = false;
        }
    } else {
        bIsActing = false;
    }
    
    // Calculate the position of the action circle.
    if ( bIsActing ) {
        actPos.x = pos.x + radius * sin( ofDegToRad( angle ) );
        actPos.y = pos.y + radius * cos( ofDegToRad( angle ) );
    }
}