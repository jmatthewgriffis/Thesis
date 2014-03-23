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

void Track::addObject( string _note, float _xPos, int _age ) {
    
    stringList.push_back( _note );
    stringList.push_back( ofToString( _xPos ) );
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
    float spacer;
    
    if ( gameState < 5 || gameState == 7) { // Start jam1.
        
        m1Bass = iScaler * 20;
        spacer = fMeasureLength / 5.333333;
        
        addObject("c3#", m1Bass-iScaler * 4, -1); // test note
        
        addObject("d2#", m1Bass, -1);
        addObject("a2#", m1Bass, -1);
        addObject("d2#", m1Bass + spacer, -1);
        addObject("a2#", m1Bass + spacer, -1);
        addObject("d2#", m1Bass + spacer * 2, -1);
        addObject("a2#", m1Bass + spacer * 2, -1);
        addObject("d2#", m1Bass + spacer * 3, -1);
        addObject("a2#", m1Bass + spacer * 3, -1);
        addObject("d2#", m1Bass + spacer * 4, -1);
        addObject("a2#", m1Bass + spacer * 4, -1);
        
        
        //riff treble
        float m1Treble = m1Bass + iScaler * 4;
        addObject("f3#", m1Treble, -1);
        addObject("d3#", m1Treble + iScaler * 4, -1);
        addObject("c3#", m1Treble + iScaler * 12, -1);
        addObject("d3#", m1Treble + iScaler * 16, -1);
        addObject("f3#", m1Treble + iScaler * 24.4, -1);
//
//        //m2
//        
//        //riff bass
//        float m2Bass = m1Bass + fMeasureLength;
//        
//        addObject("d2#", m2Bass, -1);
//        addObject("a2#", m2Bass, -1);
//        addObject("d2#", m2Bass + spacer, -1);
//        addObject("a2#", m2Bass + spacer, -1);
//        addObject("d2#", m2Bass + spacer * 2, -1);
//        addObject("a2#", m2Bass + spacer * 2, -1);
//        addObject("d2#", m2Bass + spacer * 3, -1);
//        addObject("a2#", m2Bass + spacer * 3, -1);
//        addObject("d2#", m2Bass + spacer * 4, -1);
//        addObject("a2#", m2Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m2Treble = m2Bass + iScaler * 4;
//        addObject("f3#", m2Treble, -1);
//        addObject("d3#", m2Treble + iScaler * 4, -1);
//        addObject("c3#", m2Treble + iScaler * 12, -1);
//        addObject("d3#", m2Treble + iScaler * 16, -1);
//        addObject("f3#", m2Treble + iScaler * 24, -1);
//        
//        //m3
//        
//        //riff bass
//        float m3Bass = m2Bass + fMeasureLength;
//        
//        addObject("d2#", m3Bass, -1);
//        addObject("a2#", m3Bass, -1);
//        addObject("d2#", m3Bass + spacer, -1);
//        addObject("a2#", m3Bass + spacer, -1);
//        addObject("d2#", m3Bass + spacer * 2, -1);
//        addObject("a2#", m3Bass + spacer * 2, -1);
//        addObject("d2#", m3Bass + spacer * 3, -1);
//        addObject("a2#", m3Bass + spacer * 3, -1);
//        addObject("d2#", m3Bass + spacer * 4, -1);
//        addObject("a2#", m3Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m3Treble = m3Bass + iScaler * 4;
//        addObject("f3#", m3Treble, -1);
//        addObject("d3#", m3Treble + iScaler * 4, -1);
//        addObject("c3#", m3Treble + iScaler * 12, -1);
//        addObject("d3#", m3Treble + iScaler * 16, -1);
//        addObject("f3#", m3Treble + iScaler * 24, -1);
//        
//        //m4
//        
//        //riff bass
//        float m4Bass = m3Bass + fMeasureLength;
//        
//        addObject("d2#", m4Bass, -1);
//        addObject("a2#", m4Bass, -1);
//        addObject("d2#", m4Bass + spacer, -1);
//        addObject("a2#", m4Bass + spacer, -1);
//        addObject("d2#", m4Bass + spacer * 2, -1);
//        addObject("a2#", m4Bass + spacer * 2, -1);
//        addObject("d2#", m4Bass + spacer * 3, -1);
//        addObject("a2#", m4Bass + spacer * 3, -1);
//        addObject("d2#", m4Bass + spacer * 4, -1);
//        addObject("a2#", m4Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m4Treble = m4Bass + iScaler * 4;
//        addObject("f3#", m4Treble, -1);
//        addObject("d3#", m4Treble + iScaler * 4, -1);
//        addObject("c3#", m4Treble + iScaler * 12, -1);
//        addObject("d3#", m4Treble + iScaler * 16, -1);
//        addObject("f3#", m4Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m5
//        
//        //riff bass
//        float m5Bass = m4Bass + fMeasureLength;
//        
//        addObject("c2#", m5Bass, -1);
//        addObject("g2#", m5Bass, -1);
//        addObject("c2#", m5Bass + spacer, -1);
//        addObject("g2#", m5Bass + spacer, -1);
//        addObject("c2#", m5Bass + spacer * 2, -1);
//        addObject("g2#", m5Bass + spacer * 2, -1);
//        addObject("c2#", m5Bass + spacer * 3, -1);
//        addObject("g2#", m5Bass + spacer * 3, -1);
//        addObject("c2#", m5Bass + spacer * 4, -1);
//        addObject("g2#", m5Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m5Treble = m5Bass + iScaler * 4;
//        addObject("f3#", m5Treble, -1);
//        addObject("d3#", m5Treble + iScaler * 4, -1);
//        addObject("c3#", m5Treble + iScaler * 12, -1);
//        addObject("d3#", m5Treble + iScaler * 16, -1);
//        addObject("f3#", m5Treble + iScaler * 24, -1);
//        
//        //m6
//        
//        //riff bass
//        float m6Bass = m5Bass + fMeasureLength;
//        
//        addObject("c2#", m6Bass, -1);
//        addObject("g2#", m6Bass, -1);
//        addObject("c2#", m6Bass + spacer, -1);
//        addObject("g2#", m6Bass + spacer, -1);
//        addObject("c2#", m6Bass + spacer * 2, -1);
//        addObject("g2#", m6Bass + spacer * 2, -1);
//        addObject("c2#", m6Bass + spacer * 3, -1);
//        addObject("g2#", m6Bass + spacer * 3, -1);
//        addObject("c2#", m6Bass + spacer * 4, -1);
//        addObject("g2#", m6Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m6Treble = m6Bass + iScaler * 4;
//        addObject("f3#", m6Treble, -1);
//        addObject("d3#", m6Treble + iScaler * 4, -1);
//        addObject("c3#", m6Treble + iScaler * 12, -1);
//        addObject("d3#", m6Treble + iScaler * 16, -1);
//        addObject("f3#", m6Treble + iScaler * 24, -1);
//        
//        //m7
//        
//        //riff bass
//        float m7Bass = m6Bass + fMeasureLength;
//        
//        addObject("c2#", m7Bass, -1);
//        addObject("g2#", m7Bass, -1);
//        addObject("c2#", m7Bass + spacer, -1);
//        addObject("g2#", m7Bass + spacer, -1);
//        addObject("c2#", m7Bass + spacer * 2, -1);
//        addObject("g2#", m7Bass + spacer * 2, -1);
//        addObject("c2#", m7Bass + spacer * 3, -1);
//        addObject("g2#", m7Bass + spacer * 3, -1);
//        addObject("c2#", m7Bass + spacer * 4, -1);
//        addObject("g2#", m7Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m7Treble = m7Bass + iScaler * 4;
//        addObject("f3#", m7Treble, -1);
//        addObject("d3#", m7Treble + iScaler * 4, -1);
//        addObject("c3#", m7Treble + iScaler * 12, -1);
//        addObject("d3#", m7Treble + iScaler * 16, -1);
//        addObject("f3#", m7Treble + iScaler * 24, -1);
//        
//        //m8
//        
//        //riff bass
//        float m8Bass = m7Bass + fMeasureLength;
//        
//        addObject("c2#", m8Bass, -1);
//        addObject("g2#", m8Bass, -1);
//        addObject("c2#", m8Bass + spacer, -1);
//        addObject("g2#", m8Bass + spacer, -1);
//        addObject("c2#", m8Bass + spacer * 2, -1);
//        addObject("g2#", m8Bass + spacer * 2, -1);
//        addObject("c2#", m8Bass + spacer * 3, -1);
//        addObject("g2#", m8Bass + spacer * 3, -1);
//        addObject("c2#", m8Bass + spacer * 4, -1);
//        addObject("g2#", m8Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m8Treble = m8Bass + iScaler * 4;
//        addObject("f3#", m8Treble, -1);
//        addObject("d3#", m8Treble + iScaler * 4, -1);
//        addObject("c3#", m8Treble + iScaler * 12, -1);
//        addObject("d3#", m8Treble + iScaler * 16, -1);
//        addObject("f3#", m8Treble + iScaler * 24, -1);
//        
//        // change
//        
//        //m9
//        
//        //riff bass
//        float m9Bass = m8Bass + fMeasureLength;
//        
//        addObject("d2#", m9Bass, -1);
//        addObject("a2#", m9Bass, -1);
//        addObject("d2#", m9Bass + spacer, -1);
//        addObject("a2#", m9Bass + spacer, -1);
//        addObject("d2#", m9Bass + spacer * 2, -1);
//        addObject("a2#", m9Bass + spacer * 2, -1);
//        addObject("d2#", m9Bass + spacer * 3, -1);
//        addObject("a2#", m9Bass + spacer * 3, -1);
//        addObject("d2#", m9Bass + spacer * 4, -1);
//        addObject("a2#", m9Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m9Treble = m9Bass + iScaler * 4;
//        addObject("f3#", m9Treble, -1);
//        addObject("d3#", m9Treble + iScaler * 4, -1);
//        addObject("c3#", m9Treble + iScaler * 12, -1);
//        addObject("d3#", m9Treble + iScaler * 16, -1);
//        addObject("f3#", m9Treble + iScaler * 24, -1);
//        
//        //m10
//        
//        //riff bass
//        float m10Bass = m9Bass + fMeasureLength;
//        
//        addObject("d2#", m10Bass, -1);
//        addObject("a2#", m10Bass, -1);
//        addObject("d2#", m10Bass + spacer, -1);
//        addObject("a2#", m10Bass + spacer, -1);
//        addObject("d2#", m10Bass + spacer * 2, -1);
//        addObject("a2#", m10Bass + spacer * 2, -1);
//        addObject("d2#", m10Bass + spacer * 3, -1);
//        addObject("a2#", m10Bass + spacer * 3, -1);
//        addObject("d2#", m10Bass + spacer * 4, -1);
//        addObject("a2#", m10Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m10Treble = m10Bass + iScaler * 4;
//        addObject("f3#", m10Treble, -1);
//        addObject("d3#", m10Treble + iScaler * 4, -1);
//        addObject("c3#", m10Treble + iScaler * 12, -1);
//        addObject("d3#", m10Treble + iScaler * 16, -1);
//        addObject("f3#", m10Treble + iScaler * 24, -1);
//        
//        //m11
//        
//        //riff bass
//        float m11Bass = m10Bass + fMeasureLength;
//        
//        addObject("d2#", m11Bass, -1);
//        addObject("a2#", m11Bass, -1);
//        addObject("d2#", m11Bass + spacer, -1);
//        addObject("a2#", m11Bass + spacer, -1);
//        addObject("d2#", m11Bass + spacer * 2, -1);
//        addObject("a2#", m11Bass + spacer * 2, -1);
//        addObject("d2#", m11Bass + spacer * 3, -1);
//        addObject("a2#", m11Bass + spacer * 3, -1);
//        addObject("d2#", m11Bass + spacer * 4, -1);
//        addObject("a2#", m11Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m11Treble = m11Bass + iScaler * 4;
//        addObject("f3#", m11Treble, -1);
//        addObject("d3#", m11Treble + iScaler * 4, -1);
//        addObject("c3#", m11Treble + iScaler * 12, -1);
//        addObject("d3#", m11Treble + iScaler * 16, -1);
//        addObject("f3#", m11Treble + iScaler * 24, -1);
//        
//        //m12
//        
//        //riff bass
//        float m12Bass = m11Bass + fMeasureLength;
//        
//        addObject("d2#", m12Bass, -1);
//        addObject("a2#", m12Bass, -1);
//        addObject("d2#", m12Bass + spacer, -1);
//        addObject("a2#", m12Bass + spacer, -1);
//        addObject("d2#", m12Bass + spacer * 2, -1);
//        addObject("a2#", m12Bass + spacer * 2, -1);
//        addObject("d2#", m12Bass + spacer * 3, -1);
//        addObject("a2#", m12Bass + spacer * 3, -1);
//        addObject("d2#", m12Bass + spacer * 4, -1);
//        addObject("a2#", m12Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m12Treble = m12Bass + iScaler * 4;
//        addObject("f3#", m12Treble, -1);
//        addObject("d3#", m12Treble + iScaler * 4, -1);
//        addObject("c3#", m12Treble + iScaler * 12, -1);
//        addObject("d3#", m12Treble + iScaler * 16, -1);
//        addObject("f3#", m12Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m13
//        
//        //riff bass
//        float m13Bass = m12Bass + fMeasureLength;
//        
//        addObject("c2#", m13Bass, -1);
//        addObject("g2#", m13Bass, -1);
//        addObject("c2#", m13Bass + spacer, -1);
//        addObject("g2#", m13Bass + spacer, -1);
//        addObject("c2#", m13Bass + spacer * 2, -1);
//        addObject("g2#", m13Bass + spacer * 2, -1);
//        addObject("c2#", m13Bass + spacer * 3, -1);
//        addObject("g2#", m13Bass + spacer * 3, -1);
//        addObject("c2#", m13Bass + spacer * 4, -1);
//        addObject("g2#", m13Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m13Treble = m13Bass + iScaler * 4;
//        addObject("f3#", m13Treble, -1);
//        addObject("d3#", m13Treble + iScaler * 4, -1);
//        addObject("c3#", m13Treble + iScaler * 12, -1);
//        addObject("d3#", m13Treble + iScaler * 16, -1);
//        addObject("f3#", m13Treble + iScaler * 24, -1);
//        
//        //m14
//        
//        //riff bass
//        float m14Bass = m13Bass + fMeasureLength;
//        
//        addObject("c2#", m14Bass, -1);
//        addObject("g2#", m14Bass, -1);
//        addObject("c2#", m14Bass + spacer, -1);
//        addObject("g2#", m14Bass + spacer, -1);
//        addObject("c2#", m14Bass + spacer * 2, -1);
//        addObject("g2#", m14Bass + spacer * 2, -1);
//        addObject("c2#", m14Bass + spacer * 3, -1);
//        addObject("g2#", m14Bass + spacer * 3, -1);
//        addObject("c2#", m14Bass + spacer * 4, -1);
//        addObject("g2#", m14Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m14Treble = m14Bass + iScaler * 4;
//        addObject("f3#", m14Treble, -1);
//        addObject("d3#", m14Treble + iScaler * 4, -1);
//        addObject("c3#", m14Treble + iScaler * 12, -1);
//        addObject("d3#", m14Treble + iScaler * 16, -1);
//        addObject("f3#", m14Treble + iScaler * 24, -1);
//        
//        //m15
//        
//        //riff bass
//        float m15Bass = m14Bass + fMeasureLength;
//        
//        addObject("c2#", m15Bass, -1);
//        addObject("g2#", m15Bass, -1);
//        addObject("c2#", m15Bass + spacer, -1);
//        addObject("g2#", m15Bass + spacer, -1);
//        addObject("c2#", m15Bass + spacer * 2, -1);
//        addObject("g2#", m15Bass + spacer * 2, -1);
//        addObject("c2#", m15Bass + spacer * 3, -1);
//        addObject("g2#", m15Bass + spacer * 3, -1);
//        addObject("c2#", m15Bass + spacer * 4, -1);
//        addObject("g2#", m15Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m15Treble = m15Bass + iScaler * 4;
//        addObject("f3#", m15Treble, -1);
//        addObject("d3#", m15Treble + iScaler * 4, -1);
//        addObject("c3#", m15Treble + iScaler * 12, -1);
//        addObject("d3#", m15Treble + iScaler * 16, -1);
//        addObject("f3#", m15Treble + iScaler * 24, -1);
//        
//        //m16
//        
//        //riff bass
//        float m16Bass = m15Bass + fMeasureLength;
//        
//        addObject("c2#", m16Bass, -1);
//        addObject("g2#", m16Bass, -1);
//        addObject("c2#", m16Bass + spacer, -1);
//        addObject("g2#", m16Bass + spacer, -1);
//        addObject("c2#", m16Bass + spacer * 2, -1);
//        addObject("g2#", m16Bass + spacer * 2, -1);
//        addObject("c2#", m16Bass + spacer * 3, -1);
//        addObject("g2#", m16Bass + spacer * 3, -1);
//        addObject("c2#", m16Bass + spacer * 4, -1);
//        addObject("g2#", m16Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m16Treble = m16Bass + iScaler * 4;
//        addObject("f3#", m16Treble, -1);
//        addObject("d3#", m16Treble + iScaler * 4, -1);
//        addObject("c3#", m16Treble + iScaler * 12, -1);
//        addObject("d3#", m16Treble + iScaler * 16, -1);
//        addObject("f3#", m16Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m17
//        
//        //riff bass
//        float m17Bass = m16Bass + fMeasureLength;
//        
//        addObject("b1", m17Bass, -1);
//        addObject("f2#", m17Bass, -1);
//        addObject("b1", m17Bass + spacer, -1);
//        addObject("f2#", m17Bass + spacer, -1);
//        addObject("b1", m17Bass + spacer * 2, -1);
//        addObject("f2#", m17Bass + spacer * 2, -1);
//        addObject("b1", m17Bass + spacer * 3, -1);
//        addObject("f2#", m17Bass + spacer * 3, -1);
//        addObject("b1", m17Bass + spacer * 4, -1);
//        addObject("f2#", m17Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m17Treble = m17Bass + iScaler * 4;
//        addObject("f3#", m17Treble, -1);
//        addObject("d3#", m17Treble + iScaler * 4, -1);
//        addObject("c3#", m17Treble + iScaler * 12, -1);
//        addObject("d3#", m17Treble + iScaler * 16, -1);
//        addObject("f3#", m17Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m18
//        
//        //riff bass
//        float m18Bass = m17Bass + fMeasureLength;
//        
//        addObject("c2#", m18Bass, -1);
//        addObject("g2#", m18Bass, -1);
//        addObject("c2#", m18Bass + spacer, -1);
//        addObject("g2#", m18Bass + spacer, -1);
//        addObject("c2#", m18Bass + spacer * 2, -1);
//        addObject("g2#", m18Bass + spacer * 2, -1);
//        addObject("c2#", m18Bass + spacer * 3, -1);
//        addObject("g2#", m18Bass + spacer * 3, -1);
//        addObject("c2#", m18Bass + spacer * 4, -1);
//        addObject("g2#", m18Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m18Treble = m18Bass + iScaler * 4;
//        addObject("f3#", m18Treble, -1);
//        addObject("d3#", m18Treble + iScaler * 4, -1);
//        addObject("c3#", m18Treble + iScaler * 12, -1);
//        addObject("d3#", m18Treble + iScaler * 16, -1);
//        addObject("f3#", m18Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m19
//        
//        //riff bass
//        float m19Bass = m18Bass + fMeasureLength;
//        
//        addObject("d2#", m19Bass, -1);
//        addObject("a2#", m19Bass, -1);
//        addObject("d2#", m19Bass + spacer, -1);
//        addObject("a2#", m19Bass + spacer, -1);
//        addObject("d2#", m19Bass + spacer * 2, -1);
//        addObject("a2#", m19Bass + spacer * 2, -1);
//        addObject("d2#", m19Bass + spacer * 3, -1);
//        addObject("a2#", m19Bass + spacer * 3, -1);
//        addObject("d2#", m19Bass + spacer * 4, -1);
//        addObject("a2#", m19Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m19Treble = m19Bass + iScaler * 4;
//        addObject("f3#", m19Treble, -1);
//        addObject("d3#", m19Treble + iScaler * 4, -1);
//        addObject("c3#", m19Treble + iScaler * 12, -1);
//        addObject("d3#", m19Treble + iScaler * 16, -1);
//        addObject("f3#", m19Treble + iScaler * 24, -1);
//        
//        //m20
//        
//        //riff bass
//        float m20Bass = m19Bass + fMeasureLength;
//        
//        addObject("d2#", m20Bass, -1);
//        addObject("a2#", m20Bass, -1);
//        addObject("d2#", m20Bass + spacer, -1);
//        addObject("a2#", m20Bass + spacer, -1);
//        addObject("d2#", m20Bass + spacer * 2, -1);
//        addObject("a2#", m20Bass + spacer * 2, -1);
//        addObject("d2#", m20Bass + spacer * 3, -1);
//        addObject("a2#", m20Bass + spacer * 3, -1);
//        addObject("d2#", m20Bass + spacer * 4, -1);
//        addObject("a2#", m20Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m20Treble = m20Bass + iScaler * 4;
//        addObject("f3#", m20Treble, -1);
//        addObject("d3#", m20Treble + iScaler * 4, -1);
//        addObject("c3#", m20Treble + iScaler * 12, -1);
//        addObject("d3#", m20Treble + iScaler * 16, -1);
//        addObject("f3#", m20Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m21
//        
//        //riff bass
//        float m21Bass = m20Bass + fMeasureLength;
//        
//        addObject("b1", m21Bass, -1);
//        addObject("f2#", m21Bass, -1);
//        addObject("b1", m21Bass + spacer, -1);
//        addObject("f2#", m21Bass + spacer, -1);
//        addObject("b1", m21Bass + spacer * 2, -1);
//        addObject("f2#", m21Bass + spacer * 2, -1);
//        addObject("b1", m21Bass + spacer * 3, -1);
//        addObject("f2#", m21Bass + spacer * 3, -1);
//        addObject("b1", m21Bass + spacer * 4, -1);
//        addObject("f2#", m21Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m21Treble = m21Bass + iScaler * 4;
//        addObject("f3#", m21Treble, -1);
//        addObject("d3#", m21Treble + iScaler * 4, -1);
//        addObject("c3#", m21Treble + iScaler * 12, -1);
//        addObject("d3#", m21Treble + iScaler * 16, -1);
//        addObject("f3#", m21Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m22
//        
//        //riff bass
//        float m22Bass = m21Bass + fMeasureLength;
//        
//        addObject("c2#", m22Bass, -1);
//        addObject("g2#", m22Bass, -1);
//        addObject("c2#", m22Bass + spacer, -1);
//        addObject("g2#", m22Bass + spacer, -1);
//        addObject("c2#", m22Bass + spacer * 2, -1);
//        addObject("g2#", m22Bass + spacer * 2, -1);
//        addObject("c2#", m22Bass + spacer * 3, -1);
//        addObject("g2#", m22Bass + spacer * 3, -1);
//        addObject("c2#", m22Bass + spacer * 4, -1);
//        addObject("g2#", m22Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m22Treble = m22Bass + iScaler * 4;
//        addObject("f3#", m22Treble, -1);
//        addObject("d3#", m22Treble + iScaler * 4, -1);
//        addObject("c3#", m22Treble + iScaler * 12, -1);
//        addObject("d3#", m22Treble + iScaler * 16, -1);
//        addObject("f3#", m22Treble + iScaler * 24, -1);
//        
//        //change
//        
//        //m23
//        
//        //riff bass
//        float m23Bass = m22Bass + fMeasureLength;
//        
//        addObject("d2#", m23Bass, -1);
//        addObject("a2#", m23Bass, -1);
//        addObject("d2#", m23Bass + spacer, -1);
//        addObject("a2#", m23Bass + spacer, -1);
//        addObject("d2#", m23Bass + spacer * 2, -1);
//        addObject("a2#", m23Bass + spacer * 2, -1);
//        addObject("d2#", m23Bass + spacer * 3, -1);
//        addObject("a2#", m23Bass + spacer * 3, -1);
//        addObject("d2#", m23Bass + spacer * 4, -1);
//        addObject("a2#", m23Bass + spacer * 4, -1);
//        
//        
//        //riff treble
//        float m23Treble = m23Bass + iScaler * 4;
//        addObject("f3#", m23Treble, -1);
//        addObject("d3#", m23Treble + iScaler * 4, -1);
//        addObject("c3#", m23Treble + iScaler * 12, -1);
//        addObject("d3#", m23Treble + iScaler * 16, -1);
//        addObject("f3#", m23Treble + iScaler * 24, -1);
//        
//        //m24
//        
//        //riff bass
//        float m24Bass = m23Bass + fMeasureLength;
//        
//        addObject("d2#", m24Bass, -1);
//        addObject("a2#", m24Bass, -1);
//        
//        /*addObject("d2#", m24Bass + spacer, -1);
//         addObject("a2#", m24Bass + spacer, -1);
//         addObject("d2#", m24Bass + spacer * 2, -1);
//         addObject("a2#", m24Bass + spacer * 2, -1);
//         addObject("d2#", m24Bass + spacer * 3, -1);
//         addObject("a2#", m24Bass + spacer * 3, -1);
//         addObject("d2#", m24Bass + spacer * 4, -1);
//         addObject("a2#", m24Bass + spacer * 4, -1);*/
//        
//        
//        //riff treble
//        float m24Treble = m24Bass;
//        //    float m24Treble = m24Bass + iScaler * 4;
//        addObject("d3#", m24Treble, -1);
//        /*addObject("f3#", m24Treble, -1);
//         addObject("d3#", m24Treble + iScaler * 4, -1);
//         addObject("c3#", m24Treble + iScaler * 12, -1);
//         addObject("d3#", m24Treble + iScaler * 16, -1);
//         addObject("f3#", m24Treble + iScaler * 24, -1);*/
        
        iNumMeasures = 24;
        
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
            
            if (i < 5 || i == 7 || i == 8 || i == 11) {
                addObject("c2", (fMeasureLength * i) + m1Bass, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
                addObject("g2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
            } else if (i == 5 || i == 6 || i == 10) {
                addObject("f1", (fMeasureLength * i) + m1Bass, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
                addObject("f1", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
                addObject("c2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
            } else {
                addObject("g1", (fMeasureLength * i) + m1Bass, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass, -1);
                addObject("g1", (fMeasureLength * i) + m1Bass + spacer, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass + spacer, -1);
                addObject("g1", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass + spacer * 2, -1);
                addObject("g1", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
                addObject("d2", (fMeasureLength * i) + m1Bass + spacer * 3, -1);
            }
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