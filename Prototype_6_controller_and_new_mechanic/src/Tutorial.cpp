//
//  Tutorial.cpp
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/18/14.
//
//

#include "Tutorial.h"

Tutorial::Tutorial() {
    
}

void Tutorial::addObject( string _note, float _xPos, int _age ) {
    
    stringList.push_back( _note );
    stringList.push_back( ofToString( _xPos ) );
    stringList.push_back( ofToString( _age ) );
}

vector< string >  Tutorial::setup( int _iScaler, int _iThirdOfScreen, bool _bIsLefty ) {
    
    fWriteControls( _bIsLefty );
    
    stringList.clear();
    obstacleList.clear();
    
    iScaler = _iScaler;
    iThirdOfScreen = _iThirdOfScreen;
    
    // Jump!
    float x = iScaler * 40;
    Obstacle tmp;
    tmp.setup( ofVec2f( x, iThirdOfScreen ), iScaler * 4, iScaler * 3, true );
    obstacleList.push_back( tmp );
    
    // Jump off a note to overcome an obstacle.
    float x2 = iScaler * 72;
    addObject( "d3", x2, -1 );
    Obstacle tmp2;
    tmp2.setup( ofVec2f( x2 + iScaler * 8, iThirdOfScreen ), iScaler * 4, iScaler * 6, true );
    obstacleList.push_back( tmp2 );
    
    // Record and replay a note to overcome an obstacle.
    float x3 = iScaler * 92;
    addObject( "d3", x3, -1 );
    addObject( "a3", x3 + iScaler * 29.8, -1 );
    Obstacle tmp3;
    tmp3.setup( ofVec2f( x3 + iScaler * 80, iThirdOfScreen ), iScaler * 4, iScaler * 6, true );
    obstacleList.push_back( tmp3 );
    
    // Spring off notes to get to higher notes and overcome a tall obstacle.
    float x4 = iScaler * 184;
    addObject( "d3", x4, -1 );
    //    addObject( "g4", x4 + iScaler * 8, -1 );
    addObject( "c4", x4 + iScaler * 16, -1 );
    addObject( "g3", x4 + iScaler * 28, -1 );
    
    float x5 = x4 + iScaler * 44;
    addObject( "g3", x5, -1 );
    addObject( "d3", x5 + iScaler * 4, -1 );
    //    addObject( "c5", x5 + iScaler * 8, -1 );
    Obstacle tmp4;
    tmp4.setup( ofVec2f( x5 + iScaler * 16, iThirdOfScreen ), iScaler * 4, iScaler * 12, true );
    obstacleList.push_back( tmp4 );
    
    // Record multiple notes before replaying to overcome a tall obstacle before the notes expire.
    addObject( "b3", x5 + iScaler * 24, -1 );
    addObject( "e3", x5 + iScaler * 28, -1 );
    
    float x6 = iScaler * 300;
    addObject( "b3", x6, -1 );
    addObject( "e3", x6 + iScaler * 8, -1 );
    Obstacle tmp5;
    tmp5.setup( ofVec2f( x6 + iScaler * 58, iThirdOfScreen ), iScaler * 4, iScaler * 12, true );
    obstacleList.push_back( tmp5 );
    
    // Ride a moving note to overcome a tall obstacle.
    Obstacle tmp6;
    tmp6.setup( ofVec2f( x6 + iScaler * 62, iThirdOfScreen ), iScaler * 4, iScaler * 8, true );
    obstacleList.push_back( tmp6 );
    Obstacle tmp7;
    tmp7.setup( ofVec2f( x6 + iScaler * 66, iThirdOfScreen ), iScaler * 4, iScaler * 4, true );
    obstacleList.push_back( tmp7 );
    
    float x7 = iScaler * 370;
    addObject( "b3", x7, -1 );
    //objectList[ objectList.size() - 1 ].vel.set( float( -( iScaler / 8.3333 ) ), 0.0 ); // Moved to testApp.cpp
    Obstacle tmp8;
    tmp8.setup( ofVec2f( x7 + iScaler * 30, iThirdOfScreen ), iScaler * 4, iScaler * 12, true );
    obstacleList.push_back( tmp8 );
    
    return stringList;
}

void Tutorial::draw( ofTrueTypeFont _font ) {
    
    ofTrueTypeFont helvetica = _font;
    
    float y = iScaler * 4;
    // Left and right
    helvetica.drawString( sLeft + " / " + sRight, iScaler * 8, y );
    // Up
    helvetica.drawString( sUp, iScaler * 64, y );
    // Record
    helvetica.drawString( sAltLeft, iScaler * 120, y );
    // Replay
    helvetica.drawString( sAltRight, iScaler * 152, y );
    
    // Sly note
    helvetica.drawString( "Warning: the rest is under construction.\nPlease wear a hard hat.\nBut feel free to wander into the infinite reaches.\n(Press [R] to restart.)", iScaler * 484, y );
    
    // Cue boss battle.
    helvetica.drawString( "[ENTER] here for a boss battle!", iScaler * 420, y + iScaler * 5 );
}

void Tutorial::fWriteControls( bool bIsLefty ) {
    
    // Here we store the controls as strings we can display on-screen.
    
    if ( !bIsLefty ) {
        
        // Movement
        
        sUp = "[W]";
        sLeft = "[A]";
        sDown = "[S]";
        sRight = "[D]";
        
        // Action
        
        sAltUp = "[UP]";
        sAltLeft = "[LEFT]";
        sAltDown = "[DOWN]";
        sAltRight = "[RIGHT]";
    }
    else {
        
        // Action
        
        sAltUp = "[W]";
        sAltLeft = "[A]";
        sAltDown = "[S]";
        sAltRight = "[D]";
        
        // Movement
        
        sUp = "[UP]";
        sLeft = "[LEFT]";
        sDown = "[DOWN]";
        sRight = "[RIGHT]";
    }
}