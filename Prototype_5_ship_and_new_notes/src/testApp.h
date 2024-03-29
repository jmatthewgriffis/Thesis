#pragma once

#include "ofMain.h"
#include "Object.h"
#include "Note.h"
#include "Player.h"
#include "Obstacle.h"

#define numYpos 30
#define numLines 13

/*
 So many thanks to:
 Michael Kahane (helped with the camera, among other things)
 Zach Lieberman (provided the audio examples in AVSYS last year)
 Charlie Whitney (taught me loads in Algo)
 */

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
    void fDrawRestartScreen();
    void fDrawTutorialInstructions();
    void fSetupTutorial();
    void fDrawStaff();
    void fCalcAllNotePos();
    
    // Manage the objects.
    void addObject( string _note, float _xPos, int _age );
    void addRecordedObject( string _note, ofVec2f _vel, int _age );
    void addReplayedObject( string _note, ofVec2f _vel, int _age );
    void updateObjectList();
    void fRecord( int _i );
    void fReplay();
    void testPattern();
    
    // Handle collision.
    void playerCollidesWithGround();
    void playerCollidesWithObject();
    void playerCollidesWithObstacle();
    void objectCollidesWithObstacle();
    
    // Also record the controls (left- or right-handed).
    void fWriteControls();
    
    // Store all the y-pos of the notes on the musical staff.
    vector< float > staffPosList;
    
    // Store all the obstacles.
    vector< Obstacle > obstacleList;
    
    // Store all the notes.
    vector< Object > objectList; // Enemies, objects, etc.
    vector< Object > recordedList; // Store recorded Objects for later use.
    
    Player myPlayer, myPlayer2;
    
    // Accomodate right-handed and left-handed.
    bool bIsLefty;
    
    // Prevent multiple record calls from the same action.
    bool bIsRecording;
    
    // Switch between game modes.
    int gameState, currentState;
    bool bIsDebugging;
    
    int frameRate, objectLife;
    
    // Control the staff lines' transparency.
    int iStaffAlphaMin, iStaffAlphaMax;
    float iStaffAlpha, iStaffAlphaVel;
    
    // Highlight a specific object.
    int getThisOne;
    bool bHighlightNote;
    
    // This will be used to time certain events.
    double lastTime;
    
    ofTrueTypeFont helvetica, helveticaJumbo;
    
    // Store a bunch of strings to accomodate different controls.
    string sUp, sLeft, sDown, sRight, sAltUp, sAltLeft, sAltDown, sAltRight;
    
    ofEasyCam myCam;
    
    bool bShiftIsPressed;
    
    int iThirdOfScreen;
    
    float fMeasureLength;
    
    ofImage trebleClef, bassClef, staffBracket;
};
