//
//  Note.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"

#define NUMNOTES 8

class Note {
public:
    // Notes are simply sounds, carried by Objects. They are not drawn, themselves, but only come into and out of being with their Objects, changing pitch as the Objects change yPos and changing volume in relation to the player's pos.
    
    Note();
    
    void setup( int _whichNote );
    
    void update( int _whichNote, float _vol );
    
    void storePaths();
    
    ofSoundPlayer sound;
    
    vector < string > loadList;
};