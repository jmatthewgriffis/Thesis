#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // Maintenance
    ofSetVerticalSync( true );
    ofSetFrameRate( 60 );
    ofSetCircleResolution( 100 );
    
    // Background
    ofBackground( 255 );
    
    // Make the vector of y-pos.
    staffPosSet();
    
    /*
     // Make some notes and add them to the vector of notes.
     for ( int i = 0; i < staffPosList.size(); i++ ) {
     Note tmp;
     tmp.setup( i, staffPosList[ i ] );
     noteList.push_back( tmp );
     }
     */
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Update the notes.
    if ( objectList.size() != 0 ) {
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].update();
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // Draw the staff, should be its own class.
    ofSetColor( 0 );
    //    ofLine( 0, ( ofGetHeight() / 8.0 ) * 1.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 1.0 ); // A above the staff.
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 2.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 2.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 3.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 3.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 4.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 4.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 5.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 5.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 6.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 6.0 );
    //    ofLine( 0, ( ofGetHeight() / 8.0 ) * 7.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 7.0 ); // C below the staff (middle C).
    
    
    // Draw the notes.
    if ( objectList.size() != 0 ) {
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].draw();
        }
    }
    
}

//--------------------------------------------------------------
void testApp::staffPosSet() {
    
    // Define the y-pos of each musical note and store it in the vector, starting from the bottom.
    for ( int i = 0; i < numNotes; i++ ) {
        float tmp = ofGetHeight() - ( ( ofGetHeight() / ( numNotes + 1 ) ) * ( i + 1 ) );
        staffPosList.push_back( tmp );
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    Object tmp;
    int i = int( ofRandom( 0, numNotes - 1 ));
    tmp.setup( i, staffPosList[ i ] );
    objectList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
