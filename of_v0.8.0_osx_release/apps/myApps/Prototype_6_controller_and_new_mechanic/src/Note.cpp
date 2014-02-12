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
}

void Note::setup( string _whichNote ) {
    
    sound.loadSound( "notes/" + _whichNote + ".mp3" );
    sound.setVolume(0.0f);
    sound.setLoop(false);
    sound.play();
}

void Note::update( string _whichNote, float _vol ) {
    
    sound.setVolume( _vol );
}

void Note::generatePath( string _whichNote ) {
    /*
    // Store the paths to each note in a vector so the Note instance can access them and update as needed. 0 is the bottom B.
    for ( int i = 0; i < NUMNOTES - 1; i++) {
        
        string loadLocation;
        
        string test = "hello";
        
        if (i == 0) loadLocation = "notes/"+test+".mp3";
        else if (i == 22) loadLocation = "notes/c4_middle.mp3";
        else if (i == 23) loadLocation = "notes/c4#.mp3";
        else if (i == 24) loadLocation = "notes/d4.mp3";
        else if (i == 25) loadLocation = "notes/d4#.mp3";
        else if (i == 26) loadLocation = "notes/e4.mp3";
        else if (i == 27) loadLocation = "notes/f4.mp3";
        else if (i == 28) loadLocation = "notes/f4#.mp3";
        else if (i == 29) loadLocation = "notes/g4.mp3";
        else if (i == 30) loadLocation = "notes/g4#.mp3";
        else if (i == 31) loadLocation = "notes/a4.mp3";
        else if (i == 32) loadLocation = "notes/a4#.mp3";
        else if (i == 33) loadLocation = "notes/b4.mp3";
        else if (i == 34) loadLocation = "notes/c5.mp3";
        else if (i == 35) loadLocation = "notes/c5#.mp3";
        else if (i == 36) loadLocation = "notes/d5.mp3";
        else if (i == 37) loadLocation = "notes/d5#.mp3";
        else if (i == 38) loadLocation = "notes/e5.mp3";
        else if (i == 39) loadLocation = "notes/f5.mp3";
        else if (i == 40) loadLocation = "notes/f5#.mp3";
        else if (i == 41) loadLocation = "notes/g5.mp3";
        else if (i == 42) loadLocation = "notes/g5#.mp3";
        else if (i == 43) loadLocation = "notes/a5.mp3";
        else if (i == 44) loadLocation = "notes/a5#.mp3";
        else if (i == 45) loadLocation = "notes/b5.mp3";
        
        loadList.push_back( loadLocation );
    }
     */
}