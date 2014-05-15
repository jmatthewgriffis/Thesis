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
    
    nowPlaying = false;
    messageTimer = 0;
    
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
        
        if (gameState < 7) {
            addObject("c3#", m1Bass + fMeasureLength - iScaler * 4, 1, -1); // test note
        }
        
        for (int i = 1; i <= iNumMeasures; i++) {
            
            // Treble
            myStream = 1;
            
            if (gameState < 7) {
                // Repeating riff
                if (i < iNumMeasures) {
                        addObject("f3#", (fMeasureLength * i) + m1Treble, myStream, -1);
                        addObject("d3#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("c3#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
                        addObject("d3#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                        addObject("f3#", (fMeasureLength * i) + m1Treble + iScaler * 24.4, myStream, -1);
                } else {
                    addObject("d3#", (fMeasureLength * i) + m1Bass, myStream, -1);
                }
            } else {
                if (i < iNumMeasures) {
                    // Repeating riff
                    myStream = 1;
                    //if (i < iNumMeasures /*i == 3 || i == 4 || i == 7 || i == 8 || i == 11 || i == 12 || i == 15 || i == 16*/) {
                        //addObject("f3#", (fMeasureLength * i) + m1Treble, myStream, -1);
                        addObject("d3#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("c3#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
                        addObject("d3#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                        addObject("f3#", (fMeasureLength * i) + m1Treble + iScaler * 24.4, myStream, -1);
                    //}
                    
                    myStream = 2;
                    /*if (i == 1 || i == 5 || i == 9) {
                        float mySpacer = spacer * 0.65;
                        addObject("d4#", (fMeasureLength * i) + m1Treble, myStream, -1);
                        addObject("f4", (fMeasureLength * i) + m1Treble + mySpacer, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + mySpacer * 2, myStream, -1);
                        addObject("g4#", (fMeasureLength * i) + m1Treble + mySpacer * 3, myStream, -1);
                    }
                    if (i == 2) {
                        float mySpacer = spacer * 0.65;
                        addObject("d3#", (fMeasureLength * i) + m1Treble, myStream, -1);
                        addObject("d3#", (fMeasureLength * i) + m1Treble + mySpacer, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + mySpacer * 2, myStream, -1);
                        addObject("g3#", (fMeasureLength * i) + m1Treble + mySpacer * 3, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + mySpacer * 5, myStream + 1, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + mySpacer * 6, myStream + 1, -1);
                    }
                    if (i == 6) {
                        float mySpacer = spacer * 0.65;
                        addObject("d3#", (fMeasureLength * i) + m1Treble + mySpacer * 2, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + mySpacer * 3, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + mySpacer * 4, myStream, -1);
                        addObject("g3#", (fMeasureLength * i) + m1Treble + mySpacer * 5, myStream, -1);
                        addObject("f3#", (fMeasureLength * i) + m1Treble + mySpacer * 6, myStream, -1);
                    }
                    if (i == 9) {
                        float mySpacer = spacer * 0.65;
                        myStream = 3;
                        addObject("d4#", (fMeasureLength * i) + m1Treble, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + mySpacer, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + mySpacer * 2, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + mySpacer * 3, myStream, -1);
                    }*/
                    if (i == 3) {
                        addObject("g3#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 20, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 24, myStream, -1);
                    }
                    if (i == 4) {
                        addObject("g3#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 8, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                    }
                    if (i == 5) {
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 8, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 24, myStream, -1);
                    }
                    if (i == 6) {
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * -4, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 8, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                        
                    }
                    if (i == 7) {
                        addObject("g3#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 8, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 18, myStream, -1);
                        addObject("g4#", (fMeasureLength * i) + m1Treble + iScaler * 24, myStream, -1);
                    }
                    if (i == 8) {
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 18, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 22, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 26, myStream, -1);
                    }
                    if (i == 9 || i == 11) {
                        if (i == 9) {
                            myStream = 2;
                        } else {
                            myStream = 3;
                        }
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 8, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 20, myStream, -1);
                        addObject("f4", (fMeasureLength * i) + m1Treble + iScaler * 24, myStream, -1);
                        //addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 32, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 36, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 40, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 48, myStream, -1);
                        addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 52, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 56, myStream, -1);
                    }
                    if (i == 13) {
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 0, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                        addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 8, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + m1Treble + iScaler * 20, myStream, -1);
                        addObject("f4", (fMeasureLength * i) + m1Treble + iScaler * 24, myStream, -1);
                        
                        float base = m1Treble + iScaler * 32;
                        myStream = 3;
                        addObject("c4#", (fMeasureLength * i) + base + iScaler * 8, myStream, -1);
                        addObject("g3#", (fMeasureLength * i) + base + iScaler * 10, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + base + iScaler * 16, myStream, -1);
                        addObject("f3#", (fMeasureLength * i) + base + iScaler * 17, myStream, -1);
                        addObject("g3#", (fMeasureLength * i) + base + iScaler * 22, myStream, -1);
                        addObject("d3#", (fMeasureLength * i) + base + iScaler * 23, myStream, -1);
                    }
                    if (i >= 17 && i < iNumMeasures) {
                        //myStream = 2;
                        float base = m1Treble;
                        addObject("d4#", (fMeasureLength * i) + base + iScaler * 0, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + base + iScaler * 2.5, myStream, -1);
                        addObject("c4#", (fMeasureLength * i) + base + iScaler * 8, myStream, -1);
                        addObject("g3#", (fMeasureLength * i) + base + iScaler * 10, myStream, -1);
                        addObject("a3#", (fMeasureLength * i) + base + iScaler * 16, myStream, -1);
                        addObject("f3#", (fMeasureLength * i) + base + iScaler * 17, myStream, -1);
                        addObject("g3#", (fMeasureLength * i) + base + iScaler * 22, myStream, -1);
                        if (i%2 == 0) {
                            addObject("d3#", (fMeasureLength * i) + base + iScaler * 23, myStream, -1);
                        } else {
                            addObject("a3#", (fMeasureLength * i) + base + iScaler * 23, myStream, -1);
                        }
                        
                    }
                } else {
                    addObject("d3#", (fMeasureLength * i) + m1Bass, myStream, -1);
                }
            }
            
            // Second path in treble
            /*if (gameState >= 7) {
                myStream = 2;
                if (i % 2 == 0 && i != iNumMeasures) {
                    addObject("f4#", (fMeasureLength * i) + m1Treble, myStream, -1);
                    addObject("g4#", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                    addObject("f4#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
                }
            }*/
            
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
        
        iNumMeasures = iNumMeasures - 1; // Hack to allow the finished jam position to register.
    } // End jam 1
    
    else if (gameState == 9) { // Start new jam 2
        
        iNumMeasures = 25;
        
        m1Bass = iScaler * 20 - fMeasureLength;
        m1Treble = m1Bass /*+ iScaler * 4*/;
        spacer = fMeasureLength / 8;
        
        // Treble
        myStream = 1;
        for (int i = 1; i < iNumMeasures + 1; i++) {
            if (i == 1 || i == 9) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
            }
            if (i == 2 || i == 6 || i == 10) {
                addObject("d4", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d4#", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("d4", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
            }
            if (i == 3 || i == 7 || i == 11) {
                addObject("a2#", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("c4", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
            }
            if (i == 4 || i == 8 || i == 12) {
                addObject("g3#", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
            }
            if (i == 5) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 8, myStream, -1);
            }
            if (i == 13) {
                addObject("a3", (fMeasureLength * i) + m1Treble + spacer * 0, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
                addObject("a3", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
            }
            if (i == 14) {
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("a3", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
            }
            if (i == 15) {
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
            }
            if (i == 16) {
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 0, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
            }
            if (i == 17 || i == 21) {
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 0, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
            }
            if (i == 18 || i == 22) {
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
            }
            if (i == 19 || i == 23) {
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 7, myStream, -1);
            }
            if (i == 20 || i == 24) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
            }
            if (i == 25) {
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 0, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 5, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 6, myStream, -1);
            }
        }
        
        // Bass
        myStream = 0;
        for (int i = 1; i < iNumMeasures + 1; i++) {
            if (i == 1 || i == 2 || i == 5 || i == 6 || i == 9 || i == 10) {
                addObject("g1#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("g1#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 3 || i == 4 || i == 7 || i == 8 || i == 11 || i == 12) {
                addObject("d1#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("d1#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 13 || i == 14) {
                addObject("f1", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 15) {
                addObject("g1#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("g1#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 16) {
                addObject("a1#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 17 || i == 18 || i == 21 || i == 22) {
                addObject("c2", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("c3_middle_bass", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("c3_middle_bass", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 19 || i == 23) {
                addObject("g1#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("g1#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("g2#", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("d2#", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 20 || i == 24) {
                addObject("a1#", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Bass + spacer * 4, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 5, myStream, -1);
                addObject("a2#", (fMeasureLength * i) + m1Bass + spacer * 6, myStream, -1);
                addObject("f2", (fMeasureLength * i) + m1Bass + spacer * 7, myStream, -1);
            }
            if (i == 25) {
                addObject("c2", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("c3_middle_bass", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
            }
        }
        
    } // End new jam 2
    
    else if (gameState == 5) { // Start old jam 2
        
        spacer = fMeasureLength / 4;
        m1Bass = iScaler * 20 - fMeasureLength;
        
        myStream = 2;
        
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
            
            if (i < 5 || i == 7 || i == 8 || i == 11) {
                addObject("c2", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
            } else if (i == 5 || i == 6 || i == 10) {
                addObject("f1", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
            } else {
                addObject("g1", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass, myStream, -1);
                addObject("g1", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass + spacer, myStream, -1);
                addObject("g1", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass + spacer * 2, myStream, -1);
                addObject("g1", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass + spacer * 3, myStream, -1);
            }
        }
        
        
        iNumMeasures = 12;
    } // End old jam 2.
    
    else if (gameState == 8) { // Start tutorial / level select
        
        m1Bass = iScaler * 20 - fMeasureLength;
        m1Treble = m1Bass + iScaler * 4;
        spacer = fMeasureLength / 4;
        
        iNumMeasures = 12;
        
        addObject("b2", m1Bass + fMeasureLength - iScaler * 4, 1, -1); // test note
        
        for (int i = 1; i <= iNumMeasures; i++) {
            // treble
            
            // go to jam 1
            myStream = 7;
            if (i == 11) {
                addObject("g4#", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("f4", (fMeasureLength * i) + m1Treble + iScaler * 4, myStream, -1);
                addObject("d4#", (fMeasureLength * i) + m1Treble + iScaler * 12, myStream, -1);
                addObject("f4", (fMeasureLength * i) + m1Treble + iScaler * 16, myStream, -1);
                addObject("g4#", (fMeasureLength * i) + m1Treble + iScaler * 24.4, myStream, -1);
                
                /*float nuSpacer = iScaler * 3;
                addObject("f4", (fMeasureLength * i) + m1Treble + iScaler * 24.4 + nuSpacer, myStream, -1);
                addObject("g4", (fMeasureLength * i) + m1Treble + iScaler * 24.4 + nuSpacer * 2, myStream, -1);
                addObject("a4#", (fMeasureLength * i) + m1Treble + iScaler * 24.4 + nuSpacer * 3, myStream, -1);
                addObject("b4", (fMeasureLength * i) + m1Treble + iScaler * 24.4 + nuSpacer * 4, myStream, -1);*/
            }
            
            // go to jam 2
            myStream = 9;
            if (i == 2) {
                addObject("d4", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("d4#", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("d4", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("a3#", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
            }
            
            myStream = 1;
            if (i == 1 || i == 5) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("b2", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
            }
            if (i == 2) {
                addObject("b2", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 2.5, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            }
            if (i == 3) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 0, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
            }
            if (i == 4) {
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 0, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("b2", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("b2", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            }
            if (i == 6) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 0.85, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 1.40, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 2.5, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
            }
            if (i == 7) {
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("b2", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
            }
            if (i == 8) {
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            }
            if (i == 9) {
                addObject("d3#", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
            }
            if (i == 10) {
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer * 2.5, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
            } if (i == 11 || i == 12) {
                addObject("d3#", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("b2", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                if (i == 11) {
                    addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                    addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
                }
            }
            /*else if (i == 5 || i == 6) {
                addObject("d3", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("f3", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("e3", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("d3", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            } else if (i == 7 || i == 8) {
                addObject("e3", (fMeasureLength * i) + m1Treble, myStream, -1);
                //addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("c4", (fMeasureLength * i) + m1Treble + spacer * 0.75, myStream, -1);
                addObject("g3", (fMeasureLength * i) + m1Treble + spacer * 1.25, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            } else if (i == 9) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            } else if (i == 10) {
                //addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                //addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            } else if (i == 11) {
                addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
                myStream = 2;
                addObject("g4", (fMeasureLength * i) + m1Treble + spacer * 2.5, myStream, -1);
                addObject("g4", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            } else if (i == 12) {
                myStream = 2;
                addObject("f4", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("e4", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("c4", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("d4", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("c4", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
                addObject("c4", (fMeasureLength * i) + m1Treble + spacer * 4, myStream, -1);
                myStream = 1;
                addObject("c3_middle", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c3_middle", (fMeasureLength * i) + m1Treble + spacer * 3, myStream, -1);
            }*/
            
            // bass
            /*myStream = 0;
            if (i == 1 || i == 3 || i == 7 || i == 11) {
                addObject("c2", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("e2", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
            } else if (i == 2 || i == 4 || i == 8) {
                addObject("c2", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("g2", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("e2", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
            } else if (i == 5 || i == 6) {
                addObject("a1#", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("a1#", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
            } else if (i == 9 || i == 10) {
                addObject("d2", (fMeasureLength * i) + m1Treble + spacer * 0.5, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Treble + spacer * 1, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Treble + spacer * 2, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Treble + spacer * 3.5, myStream, -1);
            } else if (i == 12) {
                addObject("e2", (fMeasureLength * i) + m1Treble, myStream, -1);
                addObject("d2", (fMeasureLength * i) + m1Treble + spacer, myStream, -1);
                addObject("c2", (fMeasureLength * i) + m1Treble + spacer * 1.5, myStream, -1);
            }*/
        }
    }
    
    return stringList;
}

void Track::update(ofVec2f _playerPos) {
    if (nowPlaying) {
        
        int speed = int(iScaler / 12.5);
        
        if (pos.x > _playerPos.x) {
            
            pos.x -= speed * 2;
        
        } else {
            
            messageTimer++;
            int timerMax = 180;
            
            if (messageTimer < timerMax) {
                pos.x = _playerPos.x;
            } else {
                pos.x -= speed;
            }
        }
        
        if (pos.x < _playerPos.x - ofGetWidth()) {
            nowPlaying = false;
        }
    }
}

void Track::draw( ofTrueTypeFont _font, ofVec2f _pos ) {
    if (gameState != 5 && gameState < 7) {
        _font.drawString( "Finished! Press R to restart.", iScaler * 800, ofGetHeight() / 2 );
    } else if (gameState > 6 && gameState != 8) {
        if (nowPlaying) {
            float margin = 4;
            ofSetColor(0);
            ofSetRectMode(OF_RECTMODE_CORNER);
            ofPushMatrix();{
                ofTranslate(pos.x, _pos.y);
                
                if (gameState == 9) {
                    ofRect(-iScaler * 6 - margin * 2, iScaler * 9.75 - margin, iScaler * 24.25 + margin * 4, iScaler * 1.5 + margin * 2);
                    ofSetColor(255);
                    _font.drawString("Now Playing: 'Nostalgic for Adventure'", -iScaler * 6, iScaler * 11);
                }
                if (gameState == 7) {
                    ofRect(-iScaler * 6 - margin * 2, iScaler * 9.75 - margin, iScaler * 23 + margin * 4, iScaler * 1.5 + margin * 2);
                    ofSetColor(255);
                    _font.drawString("Now Playing: 'No Shaman Dancing!'", -iScaler * 6, iScaler * 11);
                }
            }ofPopMatrix();
        }
    }
}