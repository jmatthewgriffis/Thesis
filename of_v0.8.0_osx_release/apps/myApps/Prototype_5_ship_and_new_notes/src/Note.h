//
//  Note.h
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#pragma once

#include "ofMain.h"

#define NUMNOTES 46

class Note {
public:
    // Notes are simply sounds, carried by Objects. They are not drawn, themselves, but only come into and out of being with their Objects, with pitch dependent on the Objects' yPos and changing volume in relation to the player's pos.
    
    Note();
    
    void setup( string _whichNote );
    
    void update( string _whichNote, float _vol );
    
    void generatePath( string _whichNote );
    
    ofSoundPlayer sound;
    
    vector < string > loadList;
};