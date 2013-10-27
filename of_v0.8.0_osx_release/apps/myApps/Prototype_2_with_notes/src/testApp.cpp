#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // Support r-restart:
    cleanup();
    
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
    
    myPlayer.setup();
}

//--------------------------------------------------------------
bool bShouldIErase( Object &a ){
    
    // Zach Lieberman showed me this method to remove an element from a vector. We create a boolean function, feed it a class, and pass a reference label that we make up (in this case 'a') so we can refer to the applicable object. Then we check for a certain condition which if met returns a boolean value of 'true.' Otherwise it returns 'false.'
    
    if ( a.destroyMe ) return true;
    else return false;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    myPlayer.update();
    
    // Update the notes.
    if ( objectList.size() != 0 ) {
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].update();
        }
    }
    
    // Following up the boolean function we created above, this oF function sorts the vector according to the values of the booleans and then removes any with a 'true' value:
    ofRemove( objectList, bShouldIErase );
}

//--------------------------------------------------------------
void testApp::draw(){
    
    /*if ( objectList.size() != 0 ) {
     cout<<objectList[0].myNote.loadList[3]<<endl;
     }*/
    
    // Draw the staff, should be its own class.
    ofSetColor( 0 );
    //    ofLine( 0, ( ofGetHeight() / 8.0 ) * 1.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 1.0 ); // A above the staff.
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 2.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 2.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 3.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 3.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 4.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 4.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 5.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 5.0 );
    ofLine( 0, ( ofGetHeight() / 8.0 ) * 6.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 6.0 );
    //    ofLine( 0, ( ofGetHeight() / 8.0 ) * 7.0, ofGetWidth(), ( ofGetHeight() / 8.0 ) * 7.0 ); // C below the staff (middle C).
    
    myPlayer.draw();
    
    // Draw the notes.
    if ( objectList.size() != 0 ) {
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].draw();
        }
    }
    
}

//--------------------------------------------------------------
void testApp::exit() {
    
    cleanup();
}

//--------------------------------------------------------------
void testApp::cleanup() {
    
    // Run this function when exiting or restarting the app, just to be safe.
    
    // Stop any music playing.
    for ( int i = 0; i < objectList.size(); i++ ) {
        
        objectList[ i ].myNote.sound.stop();
    }
    
    // Clear the vector.
    objectList.clear();
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
    
    switch ( key ) {
            // Reset
        case 'r':
        case 'R':
            setup();
            break;
            
            // Movement
        case 'w':
        case 'W':
        case OF_KEY_UP:
            myPlayer.moveU = true;
            break;
        case 'a':
        case 'A':
        case OF_KEY_LEFT:
            myPlayer.moveL = true;
            break;
        case 's':
        case 'S':
        case OF_KEY_DOWN:
            myPlayer.moveD = true;
            break;
        case 'd':
        case 'D':
        case OF_KEY_RIGHT:
            myPlayer.moveR = true;
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    switch ( key ) {
            // Movement
        case 'w':
        case 'W':
        case OF_KEY_UP:
            myPlayer.moveU = false;
            break;
        case 'a':
        case 'A':
        case OF_KEY_LEFT:
            myPlayer.moveL = false;
            break;
        case 's':
        case 'S':
        case OF_KEY_DOWN:
            myPlayer.moveD = false;
            break;
        case 'd':
        case 'D':
        case OF_KEY_RIGHT:
            myPlayer.moveR = false;
            break;
    }
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
    int i = int( ofRandom( 0, NUMNOTES - 1 ));
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
