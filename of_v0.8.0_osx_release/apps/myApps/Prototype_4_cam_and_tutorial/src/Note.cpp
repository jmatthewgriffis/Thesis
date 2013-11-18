//
//  Note.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Note.h"

Note::Note() {
    
    // Maintenance
    loadList.clear();
    storePaths();
}

void Note::setup( int _whichNote ) {
    
    // Load the appropriate sound on creation.
    sound.loadSound( loadList[ _whichNote ] );
    // Initial volume of zero.
    sound.setVolume(0.0f);
    // Loop it and play it.
    sound.setLoop(true);
    sound.play();
}

void Note::update( int _whichNote, float _vol ) {
    
    sound.setVolume( _vol );
}

void Note::storePaths() {
    
    // Store the paths to each note in a vector so the Note instance can access them and update as needed. 0 is the bottom B.
    for ( int i = 0; i < NUMNOTES - 1; i++) {
        
        string loadLocation;
        
        if (i == 1) loadLocation = "notes/middleC.mp3";
        else if (i == 2) loadLocation = "notes/d.mp3";
        else if (i == 3) loadLocation = "notes/e.mp3";
        else if (i == 4) loadLocation = "notes/f.mp3";
        else if (i == 5) loadLocation = "notes/g.mp3";
        else if (i == 6) loadLocation = "notes/a.mp3";
        else if (i == 7) loadLocation = "notes/b.mp3";
        else if (i == 8) loadLocation = "notes/highC.mp3";
        
        loadList.push_back( loadLocation );
    }
}