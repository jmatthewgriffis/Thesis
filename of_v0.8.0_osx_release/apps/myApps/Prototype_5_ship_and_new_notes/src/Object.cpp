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
    c = ofColor( 0 );
    
    // Calculate the y-pos for all the notes.
    for ( int i = 0; i < numPos; i++ ) {
        float tmp = ofGetHeight() - ( ( ofGetHeight() / ( numPos + 1 ) ) * ( i + 1 ) );
        staffPosList.push_back( tmp );
    }
}

void Object::setup( string _whichNote, int _age ) {
    
    whichNote = _whichNote;
    pos.y = fReturnYPos( _whichNote );
    age = _age;
    
    pos.x = ofGetWidth();
    
    vel.set( 0.0, 0.0 );
    
    colorTimer = 0;
    
    destroyMe = false;
    drawAttention = false;
    bIsRecorded = false;
    
    // Note stuff.
    //    myNote.setup( whichNote );
    vol = 1.0f;
}

void Object::update( ofVec2f _pos ) {
    
    fLimitLife();
    
    // Move!
    pos += vel;
    
    // Map the volume onto the distance between the player and the Object. Closer = louder.
    float tooFar = ofGetWidth() * 0.25 * 0.25;
    //    float dist = ofDist( _pos.x, _pos.y, pos.x, pos.y );
    float dist = ofDist( _pos.x, 0, pos.x, 0 );
    if ( dist > tooFar ) {
        if ( noteList.size() != 0 ) {
            noteList.erase( noteList.begin() );
        }
    } else {
        //        vol = ofMap( dist, tooFar, 0, 0.0f, 1.0f );
        fAddNote();
    }
    
    for ( int i = 0; i < noteList.size(); i++ ) {
        noteList[ i ].update( whichNote, vol );
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
    //    myNote.update( whichNote, vol );
    
    // What happens when the Object goes offscreen?
    {
        // Goes off on the right, warp to the other side.
        //float margin = 100;
        //if ( pos.x > ofGetWidth() + margin && vel.x > 0 ) {
        //pos.x = -margin;
        /*
         // Stop playing the Note just in case.
         myNote.sound.stop();
         destroyMe = true;
         */
        //}
        // Goes off on the left, warp to the other side.
        float farEnoughForRhythm = 600;
        if ( pos.x < 0 && vel.x < 0 ) pos.x = ofGetWidth() + farEnoughForRhythm;
    }
    
    // Make the Object turn transparent as it ages.
    float alpha;
    if ( age >= 0 && age <= 255 ) alpha = age;
    else alpha = 255;
    
    if ( colorTimer > 0 ) {
        colorTimer--;
    }
    
    // Set the color depending on the condition.
    if ( bIsRecorded ) {
        c = ofColor( 0, 255, 0, alpha );
        colorTimer = 10;
    } else if ( drawAttention && colorTimer == 0 ) {
        c = ofColor( 255, 0, 0, alpha );
    } else if ( colorTimer == 0 ) {
        c = ofColor( 0, alpha );
    }
    
    // Reset this so the color goes back to normal.
    bIsRecorded = false;
}

void Object::draw() {
    
    // Draw guidelines as needed to indicate A above the staff and C below.
    ofSetColor( 0 );
    /*if ( whichNote < 2 ) {
     ofLine( pos.x - guideLineLength, ofGetHeight() - ( ofGetHeight() / 8.0 ), pos.x + guideLineLength, ofGetHeight() - ( ofGetHeight() / 8.0 ) );
     } else if ( whichNote > 12 ) {
     ofLine( pos.x - guideLineLength, ofGetHeight() / 8.0, pos.x + guideLineLength, ofGetHeight() / 8.0 );
     }*/
    
    // Draw!
    ofSetColor( c );
    //    ofEllipse( pos, wide, tall);
    ofRect( pos, wide, tall);
    /*
     // Make it a whole note.
     ofSetColor( 255 );
     ofCircle( pos, tall/2 );
     */
}

void Object::fAddNote() {
    
    if ( noteList.size() > 0 ) {
        return;
    }
    
    Note tmp;
    tmp.setup( whichNote );
    noteList.push_back( tmp );
    
}

void Object::fLimitLife() {
    
    // Age the Object and destroy if it gets too old. Try not to feel too morbid.
    
    // Certain Objects live forever.
    if ( age == -1 ) return;
    // Others do not.
    if ( age > 0 ) {
        
        age--;
    } else {
        //        myNote.sound.stop();
        destroyMe = true;
    }
}

float Object::fReturnYPos( string _whichNote ) {
    
    float tmp;
    
    if ( _whichNote == "b2") {
        tmp = staffPosList[ 0 ];
    }
    else if ( _whichNote == "c3_middle" || _whichNote == "c3#") {
        tmp = staffPosList[ 1 ];
    }
    else if ( _whichNote == "d3" || _whichNote == "d3#") {
        tmp = staffPosList[ 2 ];
    }
    else if ( _whichNote == "e3") {
        tmp = staffPosList[ 3 ];
    }
    else if ( _whichNote == "f3" || _whichNote == "f3#") {
        tmp = staffPosList[ 4 ];
    }
    else if ( _whichNote == "g3" || _whichNote == "g3#") {
        tmp = staffPosList[ 5 ];
    }
    else if ( _whichNote == "a3" || _whichNote == "a3#") {
        tmp = staffPosList[ 6 ];
    }
    else if ( _whichNote == "b3") {
        tmp = staffPosList[ 7 ];
    }
    else if ( _whichNote == "c4" || _whichNote == "c4#") {
        tmp = staffPosList[ 8 ];
    }
    else if ( _whichNote == "d4" || _whichNote == "d4#") {
        tmp = staffPosList[ 9 ];
    }
    else if ( _whichNote == "e4") {
        tmp = staffPosList[ 10 ];
    }
    else if ( _whichNote == "f4" || _whichNote == "f4#") {
        tmp = staffPosList[ 11 ];
    }
    else if ( _whichNote == "g4" || _whichNote == "g4#") {
        tmp = staffPosList[ 12 ];
    }
    else if ( _whichNote == "a4" || _whichNote == "a4#") {
        tmp = staffPosList[ 13 ];
    }
    else if ( _whichNote == "b4") {
        tmp = staffPosList[ 14 ];
    }
    
    return tmp;
}