//
//  Note.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Note.h"

Note::Note() {
    
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