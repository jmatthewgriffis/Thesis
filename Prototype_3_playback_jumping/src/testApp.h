#pragma once

#include "ofMain.h"
#include "Object.h"
#include "Note.h"
#include "Player.h"

#define numNotes 15

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //--------MY STUFF STARTS HERE--------
    
    void cleanup();
    void exit(); // Technically this is built-in.
    
    void fDrawTitleScreen();
    
    // Manage the objects.
    void addObject( int _note, float _xPos, int _age );
    void addRecordedObject( int _note, ofVec2f _vel, int _age );
    void addReplayedObject( int _note, ofVec2f _vel, int _age );
    void updateObjectList();
    void fRecord( int _i );
    void fReplay();
    void testPattern();
    
    // Handle player-object collision.
    void playerCollidesWithObject();
    
    // Define all the staff positions.
    void staffPosSet();
    // Also record the controls (left- or right-handed).
    void fWriteControls();
    
    // Store all the y-pos of the notes on the musical staff.
    vector< float > staffPosList;
    
    // Store all the notes.
    vector< Object > objectList; // Enemies, objects, etc.
    vector< Object > recordedList; // Store recorded Objects for later use.
    
    Player myPlayer;
    
    // Accomodate right-handed and left-handed.
    bool bIsLefty;
    
    // Prevent multiple record calls from the same action.
    bool bIsRecording;
    bool bIsEmpty;
    
    // Switch between game modes.
    int gameState;
    
    int frameRate, objectLife;
    
    // Highlight a specific object.
    int getThisOne;
    
    // This will be used to time certain events.
    double lastTime;
    
    ofTrueTypeFont helvetica;
    
    // Store a bunch of strings to accomodate different controls.
    string sUp, sLeft, sDown, sRight, sAltUp, sAltLeft, sAltDown, sAltRight;
};
