//
//  TitleScreen.cpp
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/24/14.
//
//

#include "TitleScreen.h"

TitleScreen::TitleScreen() {
    bChoseControls = false;
    iWhichPrototype = 1;
}

void TitleScreen::draw( int iScaler, ofTrueTypeFont _font, bool bIsLefty ) {
    
    ofTrueTypeFont helvetica;
    helvetica = _font;
    
    ofSetRectMode( OF_RECTMODE_CORNER );
    
    ofSetColor( 0 );
    helvetica.drawString( "Hello! Welcome to Protoville, home of the prototype.", ofGetWidth() /2 - iScaler * 16.5, iScaler * 3 );
    helvetica.drawString( "Please choose your control affiliation.", ofGetWidth() /2 - iScaler * 12, iScaler * 5 );
    
    //--------------------
    
    if ( !bChoseControls ) {
        
        if ( bIsLefty) {
            ofSetColor( 0 );
        } else {
            ofSetColor( 0 );
            ofRect( ofGetWidth() / 2 - iScaler * 15, ofGetHeight() / 2 - iScaler * 1.5, ofGetWidth() / 2, iScaler * 2 );
            ofSetColor( 255 );
        }
        helvetica.drawString( "I am so dextrous (right-handed).", ofGetWidth() / 2 - iScaler * 11, ofGetHeight() / 2 );
        
        if ( bIsLefty) {
            ofSetColor( 0 );
            ofRect( ofGetWidth() / 2 - iScaler * 15, ofGetHeight() / 2 + iScaler * 0.75, ofGetWidth() / 2, iScaler * 2 );
            ofSetColor( 255 );
        } else {
            ofSetColor( 0 );
        }
        helvetica.drawString( "I feel quite sinister (left-handed).", ofGetWidth() / 2 - iScaler * 11, ofGetHeight() / 2 + iScaler * 2 );
    
    } else {
        ofPushMatrix();{
            ofTranslate( -iScaler * 6, -iScaler * 2 );
            ofSetColor( 0 );
            helvetica.drawString( "Proto1: Tutorial", ofGetWidth() / 2, ofGetHeight() / 2 );
            helvetica.drawString( "Proto2: Boss", ofGetWidth() / 2, ofGetHeight() / 2 + 50 );
            helvetica.drawString( "Proto3: Piano groove", ofGetWidth() / 2, ofGetHeight() / 2 + 100 );
            helvetica.drawString( "Proto4: Flight!", ofGetWidth() / 2, ofGetHeight() / 2 + 150 );
        }ofPopMatrix();
    }
    
    //--------------------
    
    ofSetColor( 0 );
    
    ofPushMatrix();{
        ofTranslate( ofGetWidth() / 2 + iScaler * 18, ofGetHeight() / 2 + iScaler );
        helvetica.drawString( "[^]", 0, 0 );
        ofRotate( 180 );
        helvetica.drawString( "[^]", float( iScaler * 0.6 ), float( iScaler * 0.68 ) );
    }ofPopMatrix();
    
    helvetica.drawString( "Headphones recommended.", ofGetWidth() /2 - iScaler * 9, ofGetHeight() - iScaler * 5 );
    helvetica.drawString( "Press ENTER to continue.", ofGetWidth() /2 - iScaler * 8.75, ofGetHeight() - iScaler * 3 );
}