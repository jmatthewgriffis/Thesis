//
//  Staff.cpp
//  Prototype_6_controller_and_clean_code
//
//  Created by J. Matthew Griffis on 2/27/14.
//
//

#include "Staff.h"

Staff::Staff() {
    
    trebleClef.loadImage( "images/clef_treble.png" ); // http://clipartist.info/SVG/CLIPARTIST.ORG/TREBLE/treble_clef_treble_clef-555px.png
    bassClef.loadImage( "images/clef_bass.png" ); // http://clipartist.info/RSS/openclipart.org/2011/April/15-Friday/bass_clef_bassclef-1979px.png
    staffBracket.loadImage( "images/staff_bracket.png" ); // http://musescore.org/sites/musescore.org/files/issues/Curly%20brace%20extends%20to%20all%20staves.png
    
    iStaffAlphaMin = 10;
    iStaffAlphaMax = 200;
    iStaffAlpha = iStaffAlphaMin;
    iStaffAlphaVel = 0.5;
    
    bHideAll = true;
    if (bHideAll) {
        bDrawStaffLines = false;
        bDrawStaffBracketAndClefs = false;
        bDrawMeasureLines = false;
        bDrawMeasureNumbers = false;
    } else {
        bDrawStaffLines = true;
        bDrawStaffBracketAndClefs = true;
        bDrawMeasureLines = true;
        bDrawMeasureNumbers = true;
    }
}

void Staff::update() {
    
    // Update the alpha for the staff lines.
    iStaffAlpha += iStaffAlphaVel;
    if ( iStaffAlpha < iStaffAlphaMin || iStaffAlpha > iStaffAlphaMax ) {
        iStaffAlphaVel *= -1;
    }
}

void Staff::draw( int iScaler, float fMeasureLength, int gameState, ofTrueTypeFont helvetica, ofTrueTypeFont helveticaJumbo, float myPlayerX ) {
    
    // Draw the staff with transparency.
    if ( gameState < 3 ) {
        ofSetColor( 0, int( iStaffAlpha ) );
    } else {
        ofSetColor(0);
    }
    
    // Establish some reference floats.
    float staffStart = ( ofGetHeight() - iScaler * 8 ) * staffBracket.getWidth() / staffBracket.getHeight();
    float xStart;
    if ( myPlayerX < staffStart + ofGetWidth() ) {
        xStart = staffStart;
    } else {
        xStart = myPlayerX - ofGetWidth();
    }
    
    ofSetLineWidth( 1 );
    
    // Draw the horizontal staff lines.
    if (bDrawStaffLines) {
        for ( int i = 2; i < 7; i++ ) {
            ofLine( xStart, iScaler * 2 * i, myPlayerX + ofGetWidth(), iScaler * 2 * i );
            ofLine( xStart, ofGetHeight() - ( iScaler * 2 * i ), myPlayerX + ofGetWidth(), ofGetHeight() - ( iScaler * 2 * i ) );
        }
    }
    
    if ( gameState >= 3 && gameState != 6 ) {
        
        // Draw initial vertical line.
        if (bDrawMeasureLines) {
            ofSetLineWidth( 3 );
            ofLine( xStart, iScaler * 4, xStart, ofGetHeight() - iScaler * 4 );
            ofSetLineWidth( 1 );
        }
        
        if (bDrawStaffBracketAndClefs ) {
            // Draw the initial bracket.
            staffBracket.draw( 0, iScaler * 4, ( ofGetHeight() - iScaler * 8 ) * staffBracket.getWidth() / staffBracket.getHeight(), ofGetHeight() - iScaler * 8 );
            // Draw the clefs.
            trebleClef.draw( iScaler * 4, iScaler * 8 - iScaler * 2 * 7 / 2.15, ( iScaler * 2 * 7 * trebleClef.getWidth() / trebleClef.getHeight() ), iScaler * 14 );
            bassClef.draw( iScaler * 4, ofGetHeight() - iScaler * 12, ( iScaler * 2 * 3.1 * bassClef.getWidth() / bassClef.getHeight() ), iScaler * 6.2 );
            
            // Draw the time signature.
            helveticaJumbo.drawString("4", iScaler * 11, iScaler * 8 );
            helveticaJumbo.drawString("4", iScaler * 11, iScaler * 12 );
            helveticaJumbo.drawString("4", iScaler * 11, ofGetHeight() - iScaler * 8 );
            helveticaJumbo.drawString("4", iScaler * 11, ofGetHeight() - iScaler * 4 );
        }
        
        if (bDrawMeasureLines) {
            for ( int i = 0; i < 30; i++ ) {
                // Draw the measure lines.
                ofLine( iScaler * 18 + fMeasureLength * i, iScaler * 4,  iScaler * 18 + fMeasureLength * i, ofGetHeight() - iScaler * 4 );
                if (bDrawMeasureNumbers) {
                    // Draw the measure number.
                    helvetica.drawString( ofToString( i + 1 ), iScaler * 19 + fMeasureLength * i, iScaler * 4);
                }
            }
        }
    }
}