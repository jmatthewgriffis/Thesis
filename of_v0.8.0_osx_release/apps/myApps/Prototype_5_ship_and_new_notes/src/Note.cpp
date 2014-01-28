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
    
    // Store the paths to each note in a vector so the Note instance can access them and update as needed. 0 is the bottom B.
    for ( int i = 0; i < NUMNOTES - 1; i++) {
        
        string loadLocation;
        
        string test = "hello";
        
        if (i == 0) loadLocation = "notes/"+test+".mp3";
        else if (i == 1) loadLocation = "notes/c4_middle.mp3";
        else if (i == 2) loadLocation = "notes/c4#.mp3";
        else if (i == 3) loadLocation = "notes/d4.mp3";
        else if (i == 4) loadLocation = "notes/d4#.mp3";
        else if (i == 5) loadLocation = "notes/e4.mp3";
        else if (i == 6) loadLocation = "notes/f4.mp3";
        else if (i == 7) loadLocation = "notes/f4#.mp3";
        else if (i == 8) loadLocation = "notes/g4.mp3";
        else if (i == 9) loadLocation = "notes/g4#.mp3";
        else if (i == 10) loadLocation = "notes/a4.mp3";
        else if (i == 11) loadLocation = "notes/a4#.mp3";
        else if (i == 12) loadLocation = "notes/b4.mp3";
        else if (i == 13) loadLocation = "notes/c5.mp3";
        else if (i == 14) loadLocation = "notes/c5#.mp3";
        else if (i == 15) loadLocation = "notes/d5.mp3";
        else if (i == 16) loadLocation = "notes/d5#.mp3";
        else if (i == 17) loadLocation = "notes/e5.mp3";
        else if (i == 18) loadLocation = "notes/f5.mp3";
        else if (i == 19) loadLocation = "notes/f5#.mp3";
        else if (i == 20) loadLocation = "notes/g5.mp3";
        else if (i == 21) loadLocation = "notes/g5#.mp3";
        else if (i == 22) loadLocation = "notes/a5.mp3";
        else if (i == 23) loadLocation = "notes/a5#.mp3";
        else if (i == 24) loadLocation = "notes/b5.mp3";
        
        loadList.push_back( loadLocation );
    }
}