//
//  Track.cpp
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/18/14.
//
//

#include "Track.h"

Track::Track() {
    
}

void Track::addObject( string _note, float _xPos, int _stream, int _age ) {
    
    stringList.push_back( _note );
    stringList.push_back( ofToString( _xPos ) );
    stringList.push_back( ofToString( _stream ) );
    stringList.push_back( ofToString( _age ) );
}

vector< string > Track::setup( int _iScaler, float _fMeasureLength, int _gameState ) {
    
    stringList.clear();
    
    iScaler = _iScaler;
    fMeasureLength = _fMeasureLength;
    gameState = _gameState;
    
    //m1
    
    //riff bass
    float m1Bass;
    float m1Treble;
    float spacer;
    int myStream;
    
    if ( gameState < 5 || gameState == 7) { // Start jam1.
        
        m1Bass = iScaler * 20 - fMeasureLength;
        m1Treble = m1Bass + iScaler * 4;
        spacer = fMeasureLength / 5.333333;
        
        iNumMeasures = 24;
        
        addObject("c3#", m1Bass + fMeasureLength - iScaler * 4, 1, -1); // test note
        
        for (int i = 1; i <= iNumMeasures; i++) {
            
            // Treble
            myStream = 1;
            
            if (i < iNumMeasures) {
                addObject("f3#", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                addObject("c3#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                addObject("f3#", (fMeasureLength * i) + m1Treble + iScaler * 24.4, myStream, -1);
            } else {
                addObject("d3#", (fMeasureLength * i) + m1Bass, myStream, -1);
            }
            
            myStream = 2;
            if (i % 2 == 0 && i != iNumMeasures) {
                addObject("f4#", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("g4#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
            }
            
            // Bass
            myStream = 0;
            
            if (i <= 4 || (i >= 9 && i <= 12) || i == 19 || i == 20 || i == 23) {
                addObject("d2#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
            } else if ((i >= 5 && i <= 8) || (i >= 13 && i <= 16) || i == 18 || i == 22) {
                addObject("c2#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("c2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("c2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("c2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("c2#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
            } else if (i == 17 || i == 21) {
                addObject("b1", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("f2#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("b1", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("f2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("b1", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("f2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("b1", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("f2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("b1", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("f2#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
            } else if (i == 24) {
                addObject("d2#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass, myStream, -1);
            }
            
        }
    } // End jam 1
    
    else if (gameState == 5) { // Start jam 2
        
        spacer = fMeasureLength / 4;
        m1Bass = iScaler * 20 - fMeasureLength;
        
        for (int i = 1; i < 13; i++) {
            
            /*
             12-bar blues.
             1-4:   C chord
             5-6:   F chord
             7-8:   C chord
             9:  G chord
             10: F chord
             11: C chord
             12: G chord
             */
            
//            if (i < 5 || i == 7 || i == 8 || i == 11) {
//                addObject("c2", (fMeasureLength * i) + m1Bass, -1);
//                addObject("g2", (fMeasureLength * i) + m1Bass, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, -1);
//                addObject("g2", (fMeasureLength * i) + m1Bass + spacer, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
//                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
//                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
//            } else if (i == 5 || i == 6 || i == 10) {
//                addObject("f1", (fMeasureLength * i) + m1Bass, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass, -1);
//                addObject("f1", (fMeasureLength * i) + m1Bass + spacer, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, -1);
//                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
//                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
//                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
//            } else {
//                addObject("g1", (fMeasureLength * i) + m1Bass, -1);
//                addObject("d2", (fMeasureLength * i) + m1Bass, -1);
//                addObject("g1", (fMeasureLength * i) + m1Bass + spacer, -1);
//                addObject("d2", (fMeasureLength * i) + m1Bass + spacer, -1);
//                addObject("g1", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
//                addObject("d2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
//                addObject("g1", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
//                addObject("d2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
//            }
        }
        
        
        iNumMeasures = 12;
    } // End jam 2.
    
    return stringList;
}

void Track::draw( ofTrueTypeFont _font ) {
    if (gameState != 5) {
        _font.drawString( "Finished! Press R to restart.", iScaler * 800, ofGetHeight() / 2 );
    }
}