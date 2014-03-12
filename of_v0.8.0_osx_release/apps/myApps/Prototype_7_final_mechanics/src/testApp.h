#pragma once

#include "ofMain.h"
#include "Object.h"
#include "Note.h"
#include "Player.h"
#include "Obstacle.h"
#include "BossBattle.h"
#include "Tutorial.h"
#include "Track.h"
#include "Staff.h"
#include "TitleScreen.h"
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
    /*void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);*/
    
    //--------MY STUFF STARTS HERE--------
    
    // Gamepad stuff
    void axisChanged(ofxGamepadAxisEvent &e);
    void buttonPressed(ofxGamepadButtonEvent &e);
    void buttonReleased(ofxGamepadButtonEvent &e);
    
    void cleanup();
    void exit();
    
    void fDrawRestartScreen();
    void fDrawGround();
    void fDrawDebugUI();
    
    void fCalcAllNotePos();
    void fCalcTrebleNotes();
    string fReturnNote( float yPos );
    void fLoadPrototype();
    
    void addObject( string _note, float _xPos, int _age );
    void addObject( vector< string > _stringList, int _numMeasures = 0, int _numReps = 1 );
    void addRecordedObject( string _note, ofVec2f _vel, int _age );
    void addReplayedObject( string _note, ofVec2f _vel, int _age );
    void updateObjectList();
    void fRecord( int _i );
    void fReplay();
    
    void fApplyGravity();
    void playerCollidesWithGroundOrSky();
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
    int getThisOne; // Highlight a specific object.
    int iThirdOfScreen;
    int iScaler; // Scale everything based on screen size.
    int iHitCounter; // How many notes hit?
    int iTotalTrebleNotes;
    
    float fMeasureLength;
    
    bool bIsLefty; // Left-handed control scheme
    bool bIsRecording; // Prevent multiple record calls from the same action.
    bool bIsDebugging;
    bool bHighlightNote;
    bool bShiftIsPressed;
    bool bUsingController;
    bool bCamZoomedIn;
    bool bLeftStickVertical; // Dumb hack for menu selection, find better way if possible.
    bool bBassOnly, bTrebleOnly; // Use to disable parts of a track.
    
    ofTrueTypeFont helvetica, helveticaJumbo;
    
    ofEasyCam myCam;
    
    // New classes:
    
    TitleScreen myTitle;
    
    Player myPlayer, myPlayer2;
    
    BossBattle myBoss;
    
    Tutorial myTutorial;
    
    Track myTrack;
    
    Staff myStaff;
};
