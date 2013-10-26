//
//  Note.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Note.h"

Note::Note( int _whichNote ) {
    
    // Maintenance
    loadList.clear();
    storePaths();
    
    // Load the appropriate sound on creation.
    sound.loadSound( loadList[ _whichNote ] );
    // Initial volume of zero.
    sound.setVolume(1.0f);
    // Loop it and play it.
    sound.setLoop(true);
    sound.play();
}

void Note::update( int _whichNote, float _vol ) {
    
}

void Note::storePaths() {
    
    // Store the paths to each note in a vector so the Note instance can access them and update as needed.
    for ( int i = 0; i < NUMNOTES-1; i++) {
        
        string loadLocation;
        
        if (i == 0) loadLocation = "notes/middleC.mp3";
        else if (i == 1) loadLocation = "notes/d.mp3";
        else if (i == 2) loadLocation = "notes/e.mp3";
        else if (i == 3) loadLocation = "notes/f.mp3";
        else if (i == 4) loadLocation = "notes/g.mp3";
        else if (i == 5) loadLocation = "notes/a.mp3";
        else if (i == 6) loadLocation = "notes/b.mp3";
        else if (i == 7) loadLocation = "notes/highC.mp3";
        
        loadList.push_back( loadLocation );
    }
}