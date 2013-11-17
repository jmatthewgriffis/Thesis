//
//  Object.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Object.h"

Object::Object() {
    
    tall = ( ofGetHeight() / 8.0 ) - 20;
    wide = tall * 1.5;
    guideLineLength = wide * 0.75;
}

void Object::setup( int _whichNote, float y ) {
    
    whichNote = _whichNote;
    pos.y = y;
    
    pos.x = ofGetWidth();
    
    vel.set( 0.0, 0.0 );
    
    destroyMe = false;
    drawAttention = false;
    
    // Note stuff.
    myNote.setup( _whichNote );
    vol = 0.0f;
}

void Object::update( ofVec2f _pos ) {
    
    // Move!
    pos += vel;
    
    // Map the volume onto the distance between the player and the Object. Closer = louder.
    float tooFar = ofGetWidth() * 0.25 * 0.25;
    //    float dist = ofDist( _pos.x, _pos.y, pos.x, pos.y );
    float dist = ofDist( _pos.x, 0, pos.x, 0 );
    if ( dist > tooFar ) {
        vol = 0.0f;
    } else {
        //        vol = ofMap( dist, tooFar, 0, 0.0f, 1.0f );
        vol = 1.0;
    }
    
    /*
     // Map the volume onto the position, getting louder as it approaches the middle and getting quieter as it moves away.
     if ( pos.x <= ofGetWidth() / 2 ) {
     vol = ofMap( pos.x, 0, ofGetWidth() / 2, 0.0f, 1.0f );
     } else {
     vol = ofMap( pos.x, ofGetWidth() / 2, ofGetWidth(), 1.0f, 0.0f );
     }
     */
    
    // Update the note.
    myNote.update( whichNote, vol );
    
    // What happens when the Object goes offscreen?
    {
        // Goes off on the right, warp to the other side.
        float margin = 100;
        if ( pos.x > ofGetWidth() + margin && vel.x > 0 ) {
            pos.x = -margin;
            /*
            // Stop playing the Note just in case.
            myNote.sound.stop();
            destroyMe = true;
             */
        }
        // Goes off on the left, warp to the other side.
        float farEnoughForRhythm = 600;
        if ( pos.x < 0 && vel.x < 0 ) pos.x = ofGetWidth() + farEnoughForRhythm;
    }
}

void Object::draw() {
    
    // Draw guidelines as needed to indicate A above the staff and C below.
    ofSetColor( 0 );
    if ( whichNote < 2 ) {
        ofLine( pos.x - guideLineLength, ofGetHeight() - ( ofGetHeight() / 8.0 ), pos.x + guideLineLength, ofGetHeight() - ( ofGetHeight() / 8.0 ) );
    } else if ( whichNote > 12 ) {
        ofLine( pos.x - guideLineLength, ofGetHeight() / 8.0, pos.x + guideLineLength, ofGetHeight() / 8.0 );
    }
    
    // Draw the notes. Red if needed, black otherwise.
    if ( drawAttention ) ofSetColor( 255, 0, 0 );
    else ofSetColor( 0 );
    
//    ofEllipse( pos, wide, tall);
    ofRect( pos, wide, tall);
    /*
     // Make it a whole note.
     ofSetColor( 255 );
     ofCircle( pos, tall/2 );
     */
}