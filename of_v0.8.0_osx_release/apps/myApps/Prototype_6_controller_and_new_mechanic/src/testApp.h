#pragma once

#include "ofMain.h"
#include "Object.h"
#include "Note.h"
#include "Player.h"
#include "Obstacle.h"
#include "ofxGamepadHandler.h"

#define numYpos 30
//#define numLines 13

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
    
    // Gamepad stuff
    void axisChanged(ofxGamepadAxisEvent &e);
    void buttonPressed(ofxGamepadButtonEvent &e);
    void buttonReleased(ofxGamepadButtonEvent &e);
    
    void cleanup();
    void exit();
    
    void fDrawTitleScreen();
    void fDrawRestartScreen();
    void fDrawStaff();
    void fDrawTutorialInstructions();
    
    void fCalcAllNotePos();
    void testPattern();
    void fSetupTutorial();
    void fWriteControls();
    
    void addObject( string _note, float _xPos, int _age );
    void addRecordedObject( string _note, ofVec2f _vel, int _age );
    void addReplayedObject( string _note, ofVec2f _vel, int _age );
    void updateObjectList();
    void fRecord( int _i );
    void fReplay();
    
    void playerCollidesWithGround();
    void playerCollidesWithObject();
    void playerCollidesWithObstacle();
    void objectCollidesWithObstacle();
    
    vector< float > staffPosList;
    vector< Obstacle > obstacleList;
    vector< Object > objectList;
    vector< Object > recordedList;
    
    int gameState, currentState;
    int frameRate;
    int objectLife;
    int iStaffAlphaMin, iStaffAlphaMax;
    int getThisOne; // Highlight a specific object.
    int iThirdOfScreen;
    int iScaler; // Scale everything based on screen size.
    
    float iStaffAlpha, iStaffAlphaVel;
    float fMeasureLength;
    
    bool bIsLefty; // Left-handed control scheme
    bool bIsRecording; // Prevent multiple record calls from the same action.
    bool bIsDebugging;
    bool bHighlightNote;
    bool bShiftIsPressed;
    
    // Store a bunch of strings to accomodate different controls.
    string sUp, sLeft, sDown, sRight, sAltUp, sAltLeft, sAltDown, sAltRight;
    
    ofTrueTypeFont helvetica, helveticaJumbo;
    
    ofImage trebleClef, bassClef, staffBracket;
    
    ofEasyCam myCam;
    
    Player myPlayer, myPlayer2;
};
