//
//  Object.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Object.h"

Object::Object() {
    
}

void Object::setup( vector< float > _staffPosList, string _whichNote, int _age ) {
    
    staffPosList = _staffPosList;
    whichNote = _whichNote;
    age = _age;
    
    tall = ( staffPosList[0] - staffPosList[2] ) * 0.8;
    wide = tall * 1.5;
    guideLineLength = wide * 0.75;
    c = ofColor( 0 );
    
    pos.set( ofGetWidth(), -2000 );
    
    vel.set( 0.0, 0.0 );
    
    colorTimer = noteTimer = 0;
    
    destroyMe = false;
    drawAttention = false;
    bIsRecorded = false;
    bIsTouched = false;
    
    // Note stuff.
    //    myNote.setup( whichNote );
    vol = 0.5f;
    noteDuration = 30;
}

void Object::update( ofVec2f _pos ) {
    
    pos.y = fReturnYPos( whichNote );
    
    fLimitLife();
    
    // Move!
    pos += vel;
    
    if ( noteTimer > 0 ) {
        noteTimer--;
    }
    
    // Map the volume onto the distance between the player and the Object. Closer = louder.
    float tooFar = ofGetWidth() * 0.25 * 0.25;
    //    float dist = ofDist( _pos.x, _pos.y, pos.x, pos.y );
    float dist = ofDist( _pos.x, 0, pos.x, 0 );
    /*if ( dist > tooFar ) {
        if ( noteList.size() != 0 ) {
            noteList.erase( noteList.begin() );
        }
    } else {
        //        vol = ofMap( dist, tooFar, 0, 0.0f, 1.0f );
        fAddNote();
    }*/
    if ( bIsTouched ) {
        fAddNote();
    } else if ( noteList.size() != 0 ) {
        if ( noteTimer <= 0 ) {
            noteList.erase( noteList.begin() );
        }
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
    } else if ( bIsTouched ) {
        c = ofColor( 255, 0 , 0, alpha );
    } else if ( drawAttention && colorTimer == 0 ) {
        c = ofColor( 255, 0, 0, alpha );
    } else if ( colorTimer == 0 ) {
        c = ofColor( 0, alpha );
    }
    
    // Reset this so the color goes back to normal.
    bIsRecorded = bIsTouched = false;
}

void Object::draw() {
    
    // Draw guidelines as needed to indicate A above the staff and C below.
    ofSetColor( 0 );
//    0 1 13 25 26
    float lineSpacer = staffPosList[0] - staffPosList[2];
    if ( pos.y == staffPosList[ 0 ]
        || pos.y == staffPosList[ 1 ] ) {
     ofLine( pos.x - guideLineLength, ofGetHeight() - lineSpacer, pos.x + guideLineLength, ofGetHeight() - lineSpacer );
     }
    else if ( pos.y == staffPosList[ 13 ]
             || pos.y == staffPosList[ 14 ] ) {
        ofLine( pos.x - guideLineLength, ofGetHeight() - lineSpacer * 7, pos.x + guideLineLength, ofGetHeight() - lineSpacer * 7 );
    }
    else if ( pos.y == staffPosList[ 15 ]
             || pos.y == staffPosList[ 16 ] ) {
        ofLine( pos.x - guideLineLength, lineSpacer * 7, pos.x + guideLineLength, lineSpacer * 7 );
     }
    else if ( pos.y == staffPosList[ 28 ]
             || pos.y == staffPosList[ 29 ] ) {
     ofLine( pos.x - guideLineLength, lineSpacer, pos.x + guideLineLength, lineSpacer );
     }
    
    // Draw!
    /*if ( pos.y + ( tall / 2 ) > ofGetHeight()) {
        ofSetColor(100);
    }
    else if ( pos.y + ( tall / 2 ) == ofGetHeight()) {
        ofSetColor(255,0,0);
    }
    else*/ ofSetColor( c );
    // Draw the note.
    ofEllipse( pos, wide, tall);
    // Draw the tail.
    ofSetLineWidth( 2 );
    if ( pos.y > staffPosList[ 14 ] ) {
        ofLine( pos.x - wide / 2, pos.y, pos.x - wide / 2, pos.y + lineSpacer * 4 - lineSpacer / 2 );
    } else {
        ofLine( pos.x + wide / 2, pos.y, pos.x + wide / 2, pos.y - ( lineSpacer * 4 - lineSpacer / 2 ) );
    }
    // Draw a sharp sign where appropriate.
    if ( ofStringTimesInString( whichNote, "#") != 0 ) {
        ofSetLineWidth( 3 );
        float scaler = 0.25;
        float dist = wide / 1.5;
        ofLine( // right vertical
               pos.x - dist,
               pos.y - lineSpacer * 1.5 * scaler,
               pos.x - dist,
               pos.y + lineSpacer * 1.5 * scaler
               );
        ofLine( // left vertical
               pos.x - dist - ( wide - dist ) * scaler,
               pos.y - lineSpacer * 1.5 * scaler,
               pos.x - dist - ( wide - dist ) * scaler,
               pos.y + lineSpacer * 1.5 * scaler
               );
        ofSetLineWidth( 8 );
        ofLine( // top horizontal
               pos.x - dist - ( wide - dist ) * scaler - ( wide * 0.2 ) * scaler,
               pos.y - ( lineSpacer / 2 ) * scaler + ( 0.1 * lineSpacer ) * scaler,
               pos.x - dist + ( wide * 0.2 ) * scaler,
               pos.y - ( lineSpacer / 2 ) * scaler - ( 0.1 * lineSpacer ) * scaler
               );
        ofLine( // bottom horizontal
               pos.x - dist - ( wide - dist ) * scaler - ( wide * 0.2 ) * scaler,
               pos.y + ( lineSpacer / 2 ) * scaler + ( 0.1 * lineSpacer ) * scaler,
               pos.x - dist + ( wide * 0.2 ) * scaler,
               pos.y + ( lineSpacer / 2 ) * scaler - ( 0.1 * lineSpacer ) * scaler
               );
    }
    ofSetLineWidth( 1 );
    //ofRect( pos, wide, tall);
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
    noteTimer = noteDuration;
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
    
    if ( _whichNote == "d1" || _whichNote == "d1#") {
        tmp = staffPosList[ 0 ];
    }
    else if ( _whichNote == "e1") {
        tmp = staffPosList[ 1 ];
    }
    else if ( _whichNote == "f1" || _whichNote == "f1#") {
        tmp = staffPosList[ 2 ];
    }
    else if ( _whichNote == "g1" || _whichNote == "g1#") {
        tmp = staffPosList[ 3 ];
    }
    else if ( _whichNote == "a1" || _whichNote == "a1#") {
        tmp = staffPosList[ 4 ];
    }
    else if ( _whichNote == "b1") {
        tmp = staffPosList[ 5 ];
    }
    else if ( _whichNote == "c2" || _whichNote == "c2#") {
        tmp = staffPosList[ 6 ];
    }
    else if ( _whichNote == "d2" || _whichNote == "d2#") {
        tmp = staffPosList[ 7 ];
    }
    else if ( _whichNote == "e2") {
        tmp = staffPosList[ 8 ];
    }
    else if ( _whichNote == "f2" || _whichNote == "f2#") {
        tmp = staffPosList[ 9 ];
    }
    else if ( _whichNote == "g2" || _whichNote == "g2#") {
        tmp = staffPosList[ 10 ];
    }
    else if ( _whichNote == "a2" || _whichNote == "a2#") {
        tmp = staffPosList[ 11 ];
    }
    else if ( _whichNote == "b2") {
        tmp = staffPosList[ 12 ];
    }
    // 13 = bass c; 14 = bass d; 15 = treble b
    else if ( _whichNote == "c3_middle" || _whichNote == "c3#") {
        tmp = staffPosList[ 16 ];
    }
    else if ( _whichNote == "d3" || _whichNote == "d3#") {
        tmp = staffPosList[ 17 ];
    }
    else if ( _whichNote == "e3") {
        tmp = staffPosList[ 18 ];
    }
    else if ( _whichNote == "f3" || _whichNote == "f3#") {
        tmp = staffPosList[ 19 ];
    }
    else if ( _whichNote == "g3" || _whichNote == "g3#") {
        tmp = staffPosList[ 20 ];
    }
    else if ( _whichNote == "a3" || _whichNote == "a3#") {
        tmp = staffPosList[ 21 ];
    }
    else if ( _whichNote == "b3") {
        tmp = staffPosList[ 22 ];
    }
    else if ( _whichNote == "c4" || _whichNote == "c4#") {
        tmp = staffPosList[ 23 ];
    }
    else if ( _whichNote == "d4" || _whichNote == "d4#") {
        tmp = staffPosList[ 24 ];
    }
    else if ( _whichNote == "e4") {
        tmp = staffPosList[ 25 ];
    }
    else if ( _whichNote == "f4" || _whichNote == "f4#") {
        tmp = staffPosList[ 26 ];
    }
    else if ( _whichNote == "g4" || _whichNote == "g4#") {
        tmp = staffPosList[ 27 ];
    }
    else if ( _whichNote == "a4" || _whichNote == "a4#") {
        tmp = staffPosList[ 28 ];
    }
    else if ( _whichNote == "b4") {
        tmp = staffPosList[ 29 ];
    }
    
    return tmp;
}