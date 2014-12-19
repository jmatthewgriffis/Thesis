//
//  Note.cpp
//  Prototype_1_Killer_Riff
//
//  Created by J. Matthew Griffis on 10/2/13.
//
//

#include "Note.h"

Note::Note() {
    
    vel = -5;
    tall = ( ofGetHeight() / 8.0 ) - 20;
    wide = tall * 1.5;
    guideLineLength = wide * 0.75;
}

void Note::setup( int _whichNote, float y ) {
    
    whichNote = _whichNote;
    pos.y = y;
    
    pos.x = ofGetWidth();
}

void Note::update() {
    
    // Move!
    pos.x += vel;
    
    // Pac-Man to the other side when off-screen.
    if ( pos.x < 0 ) {
        pos.x = ofGetWidth();
    } else if ( pos.x > ofGetWidth() ) {
        pos.x = 0;
    }
}

void Note::draw() {
    
    ofSetColor( 0 );
    ofEllipse( pos, wide, tall);
    /*
    // Make it a whole note.
    ofSetColor( 255 );
    ofCircle( pos, tall/2 );
     */
    
    // Draw guidelines as needed to indicate A above the staff and C below.
    if ( whichNote < 2 ) {
        ofLine( pos.x - guideLineLength, ofGetHeight() - ( ofGetHeight() / 8.0 ), pos.x + guideLineLength, ofGetHeight() - ( ofGetHeight() / 8.0 ) );
    } else if ( whichNote > 12 ) {
        ofLine( pos.x - guideLineLength, ofGetHeight() / 8.0, pos.x + guideLineLength, ofGetHeight() / 8.0 );
    }
}