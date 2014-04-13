#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //ofSetDataPathRoot("data/"); // Necessary for a standalone app.
    
    // Support r-restart.
    cleanup();
    staffPosList.clear();
    
    // Maintenance
    frameRate = 60;
    ofSetFrameRate( frameRate );
    ofSetVerticalSync( true );
    ofEnableAlphaBlending();
    ofSetCircleResolution( 100 );
    myCam.disableMouseInput();
    CGDisplayHideCursor(NULL);
    ofBackground( 255 );
    
    { // Straight from the ofxGamepad example.
        ofxGamepadHandler::get()->enableHotplug();
        
        //CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
        if(ofxGamepadHandler::get()->getNumPads()>0){
            bUsingController = true; // Matt: I added this.
			ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
			ofAddListener(pad->onAxisChanged, this, &testApp::axisChanged);
			ofAddListener(pad->onButtonPressed, this, &testApp::buttonPressed);
			ofAddListener(pad->onButtonReleased, this, &testApp::buttonReleased);
        } else {
            bUsingController = false; // Matt: I added this too.
        }
    } // End ofxGamepad stuff
    
    { /*
       Precision placement is key in this game, so that the music sounds right. However, there's no way to know what size screen will be used. So everything is sized based on the screen, via a scaler factor calculated using this wizardry:
       */
        iThirdOfScreen = int( ofGetHeight() / 2.25 );
        while ( iThirdOfScreen % 16 != 0 ) {
            iThirdOfScreen--;
        }
        iScaler = iThirdOfScreen / 16; // 25 on my fullscreen.
    } // End wizardry
    fCalcAllNotePos();
    
    helvetica.loadFont( "fonts/helvetica.otf", iScaler );
    helveticaJumbo.loadFont("fonts/helvetica.otf", iScaler * 4 );
    
    /* Gamestates:
     -1:    restart screen
     0:     title screen
     1:     platforming prototype
     2:     boss prototype
     3:     piano prototype
     4:     boost prototype
     5:     solo prototype
     */
    gameState = 0;
    currentState = gameState;
    
    bIsLefty = bIsRecording = bIsDebugging = bShiftIsPressed = myTitle.bChoseControls = bBassOnly = bTrebleOnly = bPlayerMakingNotes = bPlayerFellOver = false;
    bHighlightNote = false;
    bCamZoomedIn = false;
    bIsSecondPlayer = false;
    
    if ( bHighlightNote ) getThisOne = 0;
    else getThisOne = -1;
    objectLife = 7 * frameRate;
    fMeasureLength = iScaler * 6 * 5.333333;;
    myTitle.iWhichPrototype = 1;
    iTimeTillNote = 0;
    iTimeBetweenNotes = frameRate / 6;
    iLastOpacityChange = 0;
    iOpacityChangeFreq = frameRate * 0.005;
}

//--------------------------------------------------------------
bool bShouldIErase( Object &a ){
    
    if ( a.destroyMe ) return true;
    else return false;
}
//--------------------------------------------------------------
bool bShouldIErase2( StreamBit &a ){
    
    if ( a.destroyMe ) return true;
    else return false;
}
//--------------------------------------------------------------
bool my_compare( const Object &a, const Object &b ) {
    
    return a.pos.x < b.pos.x;
}

//--------------------------------------------------------------
void testApp::update(){
    
    { // Repeated from setup. Allow controller to be switched on/off during play. Thanks to Michael Kahane for leading the way on this.
        if ( bUsingController == false ) {
            //CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
            if(ofxGamepadHandler::get()->getNumPads()>0){
                bUsingController = true;
                ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
                ofAddListener(pad->onAxisChanged, this, &testApp::axisChanged);
                ofAddListener(pad->onButtonPressed, this, &testApp::buttonPressed);
                ofAddListener(pad->onButtonReleased, this, &testApp::buttonReleased);
            } else {
                bUsingController = false;
            }
        }
    } // End ofxGamepad stuff
    
    // Don't update anything else if not on the game screen.
    if ( gameState < 1 ) return;
    
    myStaff.update();
    
    fCalcTrebleNotes();
    
    // Reset essential conditionals.
    myPlayer.onSurface = false;
    myPlayer.onStream = false;
    myPlayer.closeEnough = false;
    if (bIsSecondPlayer) {
        myPlayer2.onSurface = false;
        myPlayer2.onStream = false;
        myPlayer.closeEnough = false;
    }
    
    
    if ( bHighlightNote ) {
        if ( getThisOne < 0 ) getThisOne = objectList.size() - 1;
        if ( getThisOne > objectList.size() - 1 ) getThisOne = 0;
    }
    
    fApplyGravity();
    
    // Run collision detection.
    playerCollidesWithGroundOrSky();
    playerCollidesWithObstacle();
    playerCollidesWithObject();
    if (myPlayer.bModeSurf) {
        if (!bPlayerFellOver) {
            playerCollidesWithStream();
        }
    }
    objectCollidesWithObstacle();
    
    // Calculate the player's success:
    iHitCounter = 0; // Reset this every frame.
    for ( int i = 0; i < objectList.size(); i++ ) {
        if ( objectList[ i ].bWasTouched == true ) {
            iHitCounter++;
        }
    }
    
    // Create notes on button press and limit their frequency when the button is held.
    if (iTimeTillNote > 0 ) {
        iTimeTillNote--;
    }
    if (bPlayerMakingNotes) {
        if (iTimeTillNote <= 0 ) {
            /*int myAge;
             if (gameState == 6) {
             myAge = objectLife;
             } else {
             myAge = -1;
             }*/
            ofVec2f notePos;
            if (myPlayer.bHasShip) {
                notePos = ofVec2f(myPlayer.myShip.pos.x - myPlayer.fNoteOffsetH, myPlayer.myShip.pos.y);
            } else {
                notePos = ofVec2f(myPlayer.pos.x - myPlayer.fNoteOffsetH, myPlayer.pos.y);
            }
            int stream = 1;
            if ( notePos.y <= staffPosList[ 16 ] ) {
                addObject( fReturnNote( notePos.y ), notePos.x, stream, objectLife );
            } else {
                addObject( "c3_middle", notePos.x, stream, objectLife );
            }
            objectList[objectList.size() - 1].bIsTouched = true;
            iTimeTillNote = iTimeBetweenNotes;
        }
    } else {
        iTimeTillNote = 0;
    }
    
    // Update the player(s) (duh).
    float tmp;
    if (myPlayer.bHasShip) {
        tmp = myPlayer.myShip.pos.y;
    } else {
        tmp = myPlayer.pos.y;
    }
    /*if (updateGame)*/ myPlayer.update( gameState, fReturnNote(tmp) );
    
    if ( bIsSecondPlayer ) {
        float tmp2;
        if (myPlayer2.bHasShip) {
            tmp2 = myPlayer2.myShip.pos.y;
        } else {
            tmp2 = myPlayer2.pos.y;
        }
        myPlayer2.update( gameState, fReturnNote(tmp2) );
    }
    
    // Update the notes and the stream.
    updateObjectList();
    if (myPlayer.bModeSurf && gameState != 4) {
        updateStream();
    }
    
    ofRemove( objectList, bShouldIErase );
    ofRemove( recordedList, bShouldIErase );
    ofRemove( streamBitList, bShouldIErase2 );
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //ofxGamepadHandler::get()->draw(10,10);
    
    if ( gameState == 0 ) {
        myTitle.draw( iScaler, helvetica, bIsLefty, bUsingController );
    }
    
    else if ( gameState == -1 ) {
        fDrawRestartScreen();
    }
    
    // Don't draw anything else if not on the game screen.
    else if ( gameState > 0 ) {
        myCam.begin();
        myCam.setupPerspective();
        
        ofSetColor( 0 );
        
        // Move the camera with the player, as long as it dosn't move out of bounds.
        float fZoomFactor, fMoveX, fMoveY;
        if ( bCamZoomedIn == true ) {
            fZoomFactor = -iScaler * 6;
            fMoveX = myPlayer.pos.x - ofGetWidth() / 3.0;
            fMoveY = -iScaler * 10;
        } else {
            fZoomFactor = 0;
            fMoveX = myPlayer.pos.x - ofGetWidth() / 2.0;
            fMoveY = 0;
        }
        if ( fMoveX < 0 || gameState == 2 || gameState == 6 ) {
            myCam.move( 0, fMoveY, fZoomFactor );
        } else if ( fMoveX >= 0 ) {
            myCam.move( fMoveX, fMoveY, fZoomFactor );
        }
        
        float halfOfScreen = ofGetWidth() * 0.75;
        camLeft = myCam.getPosition().x - halfOfScreen;
        camRight = myCam.getPosition().x + halfOfScreen;
        
        myStaff.draw( iScaler, fMeasureLength, gameState, helvetica, helveticaJumbo, myPlayer.pos.x );
        fDrawDebugUI();
        fDrawGround();
        
        if ( gameState == 1 ) {
            myTutorial.draw( helvetica );
        } else if ( gameState == 2 ) {
            myBoss.draw( helvetica );
        } else {
            myTrack.draw( helvetica );
        }
        
        // Draw the notes and stream.
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].draw();
        }
        for (int i = 0; i < streamBitList.size(); i++) {
            streamBitList[i].draw();
        }
        
        // Draw the obstacles.
        for ( int i = 0; i < obstacleList.size(); i++ ) {
            obstacleList[ i ].draw();
        }
        
        myPlayer.draw( helvetica, recordedList );
        if ( bIsSecondPlayer ) {
            myPlayer2.draw( helvetica, recordedList );
        }
        
        if ( gameState == 4 ) {
            ofSetColor( 0 );
            helvetica.drawString( "Notes hit: " + ofToString( iHitCounter ) + "/" + ofToString( iTotalTrebleNotes ), myPlayer.pos.x + iScaler, iScaler * 2 );
        }
        
        myCam.end();
    }
}

//--------------------------------------------------------------
void testApp::fLoadPrototype() {
    
    // Maintenance.
    cleanup();
    bBassOnly = bTrebleOnly = bIsSecondPlayer = bPlayerFellOver = false;
    float fZoomedInX = iScaler * 7.5;
    iLastOpacityChange = ofGetElapsedTimef(); // Reset the clock.
    
    // Setup player.
    myPlayer.setup( gameState, frameRate, iScaler, bUsingController, ofVec2f( iScaler * 4, iThirdOfScreen ), staffPosList );
    
    //------------
    
    if ( gameState == 1 ) {
        // Tutorial
        bCamZoomedIn = true;
        
        myPlayer.pos.x = fZoomedInX;
        addObject( myTutorial.setup( iScaler, iThirdOfScreen, bIsLefty ), 1 );
        obstacleList = myTutorial.obstacleList;
        
        objectList[ objectList.size() - 1 ].vel.set( float( -( iScaler / 8.3333 ) ), 0.0 );
        
    } else if ( gameState == 2 ) {
        // Boss
        bCamZoomedIn = true;
        
        myPlayer.pos.x = fZoomedInX;
        addObject( myBoss.setup( iScaler, fMeasureLength ), 1 );
        
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].vel.set( float( -( iScaler / 5.0 ) ), 0.0 );
        }
        
        bHighlightNote = true;
        getThisOne = 0;
        
    } else if (gameState == 3) {
        // Piano groove
        bCamZoomedIn = false;
        
        bIsSecondPlayer = true;
        myPlayer2.setup( gameState, frameRate, iScaler, bUsingController, ofVec2f( iScaler * 4, staffPosList[ 7 ] ), staffPosList );
        
        float numReps = 1;
        addObject( myTrack.setup( iScaler, fMeasureLength, gameState ), myTrack.iNumMeasures, numReps );
        
    } else if (gameState == 4) {
        // Flight!
        bCamZoomedIn = true;
        
        float numReps = 1;
        addObject( myTrack.setup( iScaler, fMeasureLength, gameState ), myTrack.iNumMeasures, numReps );
        
    } else if (gameState == 5) {
        // Solo!
        bCamZoomedIn = false;
        
        float numReps = 4;
        addObject( myTrack.setup( iScaler, fMeasureLength, gameState ), myTrack.iNumMeasures, numReps );
        
    } else if (gameState == 6) {
        // Solo--gym
        bCamZoomedIn = true;
        
    } else if (gameState == 7 || gameState == 8) {
        // Surfin' USA
        bCamZoomedIn = true; // find me
        
        if (gameState == 7) myPlayer.vel.y = -iScaler * 0.3;
        else if (gameState == 8) myPlayer.vel.y = -iScaler * 0.4;
        
        float numReps = 1;
        addObject( myTrack.setup( iScaler, fMeasureLength, gameState ), myTrack.iNumMeasures, numReps );
        
    }
    
    //------------
    
    // Remove notes if required.
    for ( int i = 0; i < objectList.size(); i++ ) {
        if ( bBassOnly == true ) {
            if ( objectList[ i ].pos.y < staffPosList[ 14 ] ) {
                objectList[ i ].destroyMe = true;
            }
        }
        if ( bTrebleOnly == true ) {
            if ( objectList[ i ].pos.y > staffPosList[ 15 ] ) {
                objectList[ i ].destroyMe = true;
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::fApplyGravity() {
    
    if ( gameState != 3 && gameState != 5 ) {
        
        float fGravity, fGravFactor, fBaseGrav;
        fBaseGrav = iScaler * 0.012;
        
        if ( gameState < 3 ) {
            fGravFactor = fBaseGrav;
        } else if ( gameState == 4 ) {
            fGravFactor = fBaseGrav * 0.65;
        } else if ( gameState == 7 || gameState == 8 ) {
            fGravFactor = fBaseGrav * 0.65;
        }
        
        if ( myPlayer.vel.y <= 0 ) {
            fGravity = fGravFactor;
        } else if ( gameState < 3 ) {
            fGravity = fGravFactor * 3;
        } else if ( gameState == 4 ) {
            fGravity = fGravFactor * 0.4;
        } else if ( gameState == 7 || gameState == 8 ) {
            fGravity = fGravFactor * 1.25;
        }
        
        myPlayer.applyForce( ofVec2f( 0.0, fGravity ) );
    }
}

//--------------------------------------------------------------

// Again, from the ofxGamepad example:

void testApp::axisChanged(ofxGamepadAxisEvent& e) {
    
    float fStickBuffer = 0.125;
    
    //-------------------------------------------
    
    // Left stick--up and down
    if ( e.axis == 3 ) {
        
        // Menu selection
        if ( gameState == 0 ) {
            if ( abs( e.value ) > fStickBuffer * 3 ) {
                if ( bLeftStickVertical == false ) {
                    if ( !myTitle.bChoseControls ) {
                        bIsLefty = !bIsLefty;
                        bLeftStickVertical = true;
                    } else {
                        if ( e.value < 0 ) {
                            myTitle.iWhichPrototype--;
                            bLeftStickVertical = true;
                        } else {
                            myTitle.iWhichPrototype++;
                            bLeftStickVertical = true;
                        }
                    }
                }
            } else {
                bLeftStickVertical = false;
            }
        }
        
        // Piano movement
        else if ( gameState == 3 ) {
            // Bass hand vertical movement
            if ( abs( e.value ) > fStickBuffer ) {
                //myPlayer2.vel.y = ofMap( e.value, -1, 1, -myPlayer2.maxVel, myPlayer2.maxVel );
                if ( e.value < 0 ) {
                    myPlayer2.up = true;
                    myPlayer2.down = false;
                } else if ( e.value > 0 ) {
                    myPlayer2.up = false;
                    myPlayer2.down = true;
                }
            } else {
                //myPlayer2.vel.y = 0;
                myPlayer2.up = false;
                myPlayer2.down = false;
            }
        }
        
        // Flap wings
        else if ( gameState == 4 ) {
            if ( abs( e.value ) > fStickBuffer ) {
                if ( e.value < 0 ) {
                    float fJumpMin = -iScaler * 0.25;
                    float fJumpMax = -iScaler * 0.75;
                    myPlayer.jumpVel = ofMap( e.value, -0.125, -1, fJumpMin, fJumpMax );
                    myPlayer.up = true;
                } else if ( e.value > 0 ) {
                    float fJumpMin = iScaler * 0.25;
                    float fJumpMax = iScaler * 0.75;
                    myPlayer.jumpVel = ofMap( e.value, 0.125, 1, fJumpMin, fJumpMax );
                    myPlayer.up = true;
                }
            } else {
                myPlayer.up = false;
            }
        }
        
        // Note-making ship
        else if ( gameState >= 5 && myPlayer.bModeFlight ) {
            if ( abs( e.value ) > fStickBuffer ) {
                //myPlayer.vel.y = ofMap( e.value, -1, 1, -myPlayer.maxVel, myPlayer.maxVel );
                if ( e.value < 0 ) {
                    myPlayer.up = true;
                    myPlayer.down = false;
                } else if ( e.value > 0 ) {
                    myPlayer.up = false;
                    myPlayer.down = true;
                }
            } else {
                //myPlayer.vel.y = 0;
                myPlayer.up = false;
                myPlayer.down = false;
            }
        }
    }
    
    //-------------------------------------------
    
    // Left stick--left and right
    if (e.axis == 2) {
        if ( gameState == 6 ) {
            if ( abs( e.value ) > fStickBuffer ) {
                if ( e.value < 0 ) {
                    myPlayer.left = true;
                    myPlayer.right = false;
                } else if ( e.value > 0 ) {
                    myPlayer.left = false;
                    myPlayer.right = true;
                }
            } else {
                myPlayer.left = false;
                myPlayer.right = false;
            }
        }
        else if ( myPlayer.bModeSurf && gameState != 4 ) {
            if ( abs( e.value ) > fStickBuffer * 2 ) {
                if ( e.value < 0 ) { // find me
                    myPlayer.myShip.bTiltUpward = true;
                    myPlayer.myShip.bTiltDownward = false;
                } else if ( e.value > 0 ) {
                    myPlayer.myShip.bTiltUpward = false;
                    myPlayer.myShip.bTiltDownward = true;
                }
            } else {
                myPlayer.myShip.bTiltUpward = false;
                myPlayer.myShip.bTiltDownward = false;
            }
        }
    }
    
    //-------------------------------------------
    
    // Right stick--up and down
    if ( e.axis == 5 ) {
        
        // Piano movement
        if ( gameState == 3 ) {
            // P1 vertical movement
            if ( abs( e.value ) > fStickBuffer ) {
                //myPlayer.vel.y = ofMap( e.value, -1, 1, -myPlayer.maxVel, myPlayer.maxVel );
                if ( e.value < 0 ) {
                    myPlayer.up = true;
                    myPlayer.down = false;
                } else if ( e.value > 0 ) {
                    myPlayer.up = false;
                    myPlayer.down = true;
                }
            } else {
                //myPlayer.vel.y = 0;
                myPlayer.up = false;
                myPlayer.down = false;
            }
        }
    }
}

void testApp::buttonReleased(ofxGamepadButtonEvent& e) {
    // This is actually buttonPressed. Why? Who knows.
    
    // "A" button
    if ( e.button == 11 ) {
        
        if ( gameState == 0 ) {
            if ( !myTitle.bChoseControls ) {
                myTitle.bChoseControls = true;
            } else {
                gameState = myTitle.iWhichPrototype;
                currentState = gameState;
                fLoadPrototype();
            }
        } else if (myPlayer.bCanMakeNotes) {
            bPlayerMakingNotes = true;
        }
        
        if (myPlayer.bModeSurf && gameState != 4) {
            myPlayer.up = true;
        }
    }
}

void testApp::buttonPressed(ofxGamepadButtonEvent& e) {
    // This is actually buttonReleased. Why? Who knows.
    
    // "A" button
    if ( e.button == 11 ) {
        
        if (myPlayer.bCanMakeNotes) {
            bPlayerMakingNotes = false;
        }
        
        if (myPlayer.bModeSurf && gameState != 4) {
            myPlayer.up = false;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch ( key ) {
            //find me
        case 'z':
            myPlayer.allowMove = !myPlayer.allowMove;
            break;
        case 'x':
            myPlayer.maxVel *= -1;
            break;
        case 'k':
            myPlayer.tmpAngle-=5;
            break;
        case 'l':
            myPlayer.tmpAngle+=5;
            break;
            // Reset
        case 'r':
        case 'R':
            /*if ( bShiftIsPressed ) {
             setup();
             gameState = 1;
             }*/
            //else {
            if ( gameState > 0 ) {
                currentState = gameState;
                gameState = -1;
            }
            //}
            break;
            
        case 'y':
        case 'Y':
            if ( gameState == -1 ) {
                setup();
            }
            break;
            
        case 'n':
        case 'N':
            if ( gameState == -1 ) {
                gameState = currentState;
            }
            break;
            
            // Proceed on menu.
        case OF_KEY_RETURN:
            if ( gameState == 0 ) {
                if ( !myTitle.bChoseControls ) {
                    myTitle.bChoseControls = true;
                } else {
                    gameState = myTitle.iWhichPrototype;
                    currentState = gameState;
                    fLoadPrototype();
                }
            }
            // Go to boss battle if player has reached end of tutorial.
            else if ( gameState == 1 && myPlayer.pos.x > obstacleList[ obstacleList.size() - 1 ].pos.x + obstacleList[ obstacleList.size() - 1 ].wide + iScaler * 16 ) {
                //else if ( gameState == 1 ) {
                gameState = 2;
                fLoadPrototype();
            }
            break;
            
        case OF_KEY_SHIFT:
            bShiftIsPressed = true;
            break;
            
            //----------------------------------------------------
            // Movement and action (depends on the control scheme).
            
            // UP
        case 'w':
        case 'W':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    //
                } else {
                    myPlayer.up = true;
                }
            } else if ( bUsingController == false ) {
                if (bIsSecondPlayer) {
                    myPlayer2.up = true;
                }
            }
            break;
        case OF_KEY_UP:
            if ( gameState == 0 ) {
                if ( !myTitle.bChoseControls ) {
                    bIsLefty = !bIsLefty;
                } else {
                    myTitle.iWhichPrototype--;
                }
            } else if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.up = true;
                } else {
                    //
                }
            } else if ( bUsingController == false ) {
                myPlayer.up = true;
            }
            break;
            
            // LEFT
        case 'a':
        case 'A':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.record = true;
                } else {
                    myPlayer.left = true;
                }
            }
            break;
        case OF_KEY_LEFT:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.left = true;
                } else {
                    myPlayer.record = true;
                }
            } else if (gameState == 6) {
                myPlayer.left = true;
            } else if (!bUsingController && myPlayer.bModeSurf && gameState != 4) {
                myPlayer.myShip.bTiltUpward = true;
            }
            break;
            
            // DOWN
        case 's':
        case 'S':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    //
                } else {
                    myPlayer.down = true;
                }
            } else if ( bUsingController == false ) {
                if (bIsSecondPlayer) {
                    myPlayer2.down = true;
                }
            }
            break;
        case OF_KEY_DOWN:
            if ( gameState == 0 ) {
                if ( !myTitle.bChoseControls ) {
                    bIsLefty = !bIsLefty;
                } else {
                    myTitle.iWhichPrototype++;
                }
            } else if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.down = true;
                } else {
                    //
                }
            } else if ( bUsingController == false ) {
                myPlayer.down = true;
            }
            break;
            
            // RIGHT
        case 'd':
        case 'D':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.replay = true;
                    if ( myPlayer.bAllowReplay ) {
                        fReplay();
                    }
                }
                else {
                    myPlayer.right = true;
                }
            }
            break;
        case OF_KEY_RIGHT:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.right = true;
                } else {
                    myPlayer.replay = true;
                    if ( myPlayer.bAllowReplay ) {
                        fReplay();
                    }
                }
            } else if (gameState == 6) {
                myPlayer.right = true;
            } else if (!bUsingController && myPlayer.bModeSurf && gameState != 4) {
                myPlayer.myShip.bTiltDownward = true;
            }
            break;
            
        case ' ':
            if (myPlayer.bCanMakeNotes && !bUsingController) {
                bPlayerMakingNotes = true;
            }
            break;
            
            // End movement and action.
            //----------------------------------------------------
            
            // Debug mode-----------------------------------------
        {
        case '0':
            bIsDebugging = !bIsDebugging;
            break;
            
        case '[':
            if ( bIsDebugging ) {
                getThisOne--;
            }
            break;
            
        case ']':
            if ( bIsDebugging ) {
                getThisOne++;
            }
            break;
            
        case 'p':
            if ( bIsDebugging ) {
                if ( gameState == 0 ) gameState = 1;
                else if ( gameState == 1 ) gameState = 0;
            }
            break;
            
        case '1':
            if ( bIsDebugging ) {
                // Turn Object movement on and off.
                for ( int i = 0; i < objectList.size(); i++ ) {
                    if ( objectList[ i ].vel == ofVec2f ( 0 ) ) {
                        objectList[ i ].vel.set( float( -( iScaler / 5.0 ) ), 0 );
                    } else {
                        objectList[ i ].vel.set( 0 );
                    }
                }
            }
            break;
        }
            // End debug------------------------------------------
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
    switch ( key ) {
            
        case OF_KEY_SHIFT:
            bShiftIsPressed = false;
            break;
            
            //----------------------------------------------------
            // Movement and action (depends on the control scheme).
            
            // UP
        case 'w':
        case 'W':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    //
                } else {
                    myPlayer.up = false;
                    myPlayer.allowJump = true;
                }
            } else {
                if (bIsSecondPlayer) {
                    myPlayer2.up = false;
                }
            }
            break;
        case OF_KEY_UP:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.up = false;
                    myPlayer.allowJump = true;
                }
            } else if ( bUsingController == false ) {
                myPlayer.up = false;
            }
            break;
            
            // LEFT
        case 'a':
        case 'A':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.record = false;
                }
                else {
                    myPlayer.left = false;
                }
            }
            break;
        case OF_KEY_LEFT:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.left = false;
                } else {
                    myPlayer.record = false;
                }
            } else if (gameState == 6) {
                myPlayer.left = false;
            } else if (!bUsingController && myPlayer.bModeSurf && gameState != 4) {
                myPlayer.myShip.bTiltUpward = false;
            }
            break;
            
            // DOWN
        case 's':
        case 'S':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    //
                } else {
                    myPlayer.down = false;
                }
            } else {
                if (bIsSecondPlayer) {
                    myPlayer2.down = false;
                }
            }
            break;
        case OF_KEY_DOWN:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.down = false;
                } else {
                    //
                }
            } else if ( bUsingController == false ){
                myPlayer.down = false;
            }
            break;
            
            // RIGHT
        case 'd':
        case 'D':
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.replay = false;
                } else {
                    myPlayer.right = false;
                }
            }
            break;
        case OF_KEY_RIGHT:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.right = false;
                } else {
                    myPlayer.replay = false;
                }
            } else if (gameState == 6) {
                myPlayer.right = false;
            } else if (!bUsingController && myPlayer.bModeSurf && gameState != 4) {
                myPlayer.myShip.bTiltDownward = false;
            }
            break;
            
        case ' ':
            if (myPlayer.bCanMakeNotes && !bUsingController) {
                bPlayerMakingNotes = false;
            }
            break;
            
            // End movement and action.
            //----------------------------------------------------
    }
}

//--------------------------------------------------------------
void testApp::updateObjectList() {
    
    for ( int i = 0; i < objectList.size(); i++ ) {
        
        // Detect for collision with player's recorder and check if a note has already been recorded in this action.
        
        if ( myPlayer.bIsRecording ) {
            
            ofVec2f dist = myPlayer.actPos - objectList[ i ].pos;
            if ( dist.lengthSquared() < ( ( iScaler * 2 ) * ( iScaler * 2 ) ) ) {
                
                fRecord( i );
                
                // Advance the counter if recorded a selected note.
                if ( bHighlightNote && i == getThisOne) getThisOne++;
            }
        } else {
            bIsRecording = false;
        }
        
        // Highlight a specific Object.
        if ( i == getThisOne ) objectList[ i ].drawAttention = true;
        else objectList[ i ].drawAttention = false;
        
        // Make volume dependent on player performance.
        if ( objectList[ i ].pos.y <= objectList[ i ].staffPosList[ 13 ] ) {
            //objectList[ i ].vol = ofMap( myPlayer.fHealth, 0, myPlayer.fHealthMax, 0.0, 1.0 );
        } else {
            //objectList[ i ].vol = ofMap( myPlayer2.fHealth, 0, myPlayer2.fHealthMax, 0.0, 1.0 );
        }
        
        objectList[ i ].update( gameState, myPlayer.pos );
    }
}

//--------------------------------------------------------------
int testApp::checkNextStreamNote(int _i) {
    
    int tmp;
    bool bIsNoteToRight = false; // Check for existing notes that are offscreen.
    vector <Object> tmpList;
    
    for (int i = 0; i < objectList.size(); i++) {
        
        // Find the notes in the same stream and to the right of the current note.
        if (objectList[i].whichStream == objectList[_i].whichStream
            && objectList[i].pos.x > objectList[_i].pos.x) {
            bIsNoteToRight = true;
            //Make sure onscreen.
            if (objectList[i].pos.x > camLeft && objectList[i].pos.x < camRight) {
                // Store them in the vector.
                tmpList.push_back(objectList[i]);
                // Also store the index value from the primary object vector.
                tmpList[tmpList.size() - 1].my_i = i;
            }
        }
    }
    
    // Sort the vector by xPos.
    ofSort(tmpList,my_compare);
    // First element is the one we want.
    if (tmpList.size() > 0) {
        tmp = tmpList[0].my_i;
    } else if (bIsNoteToRight) {
        tmp = -2;
    } else {
        tmp = -1;
    }
    return tmp;
}

//--------------------------------------------------------------
int testApp::checkPrevStreamNote(int _i) {
    
    int tmp;
    bool bIsNoteToLeft = false; // Check for existing notes that are offscreen.
    vector <Object> tmpList;
    
    for (int i = 0; i < objectList.size(); i++) {
        
        // Find the notes in the same stream and to the left of the current note.
        if (objectList[i].whichStream == objectList[_i].whichStream
            && objectList[i].pos.x < objectList[_i].pos.x) {
            bIsNoteToLeft = true;
            //Make sure onscreen.
            if (objectList[i].pos.x > camLeft && objectList[i].pos.x < camRight) {
                // Store them in the vector.
                tmpList.push_back(objectList[i]);
                // Also store the index value from the primary object vector.
                tmpList[tmpList.size() - 1].my_i = i;
            }
        }
    }
    
    // Sort the vector by xPos.
    ofSort(tmpList,my_compare);
    // Last element is the one we want.
    if (tmpList.size() > 0) {
        tmp = tmpList[tmpList.size() - 1].my_i;
    } else if (bIsNoteToLeft) {
        tmp = -2;
    } else {
        tmp = -1;
    }
    return tmp;
}

//--------------------------------------------------------------
float testApp::checkNextStreamAngle(ofVec2f _currentNotePos) {
    
    float tmp;
    
    for (int i = 0; i < streamBitList.size(); i++) {
        // Find the streamBit that begins next positionally (may not be the next one in the vector).
        ofVec2f nextBitLeftEdge = streamBitList[i].pos - streamBitList[i].slope * 0.5;
        if (nextBitLeftEdge == _currentNotePos) {
            tmp = streamBitList[i].angle;
            return tmp;
        }
    }
}

//--------------------------------------------------------------
float testApp::checkPrevStreamAngle(ofVec2f _currentNotePos) {
    
    float tmp;
    
    for (int i = 0; i < streamBitList.size(); i++) {
        // Find the streamBit that ends previously positionally (may not be the previous one in the vector).
        ofVec2f prevBitRightEdge = streamBitList[i].pos + streamBitList[i].slope * 0.5;
        if (prevBitRightEdge == _currentNotePos) {
            tmp = streamBitList[i].angle;
            return tmp;
        }
    }
}

//--------------------------------------------------------------
void testApp::updateStream() {
    // Stream is generated dynamically based on the notes onscreen.
    float closeEnoughToTouch = powf(fMeasureLength * 0.33, 2);
    
    for (int i = 0; i < objectList.size(); i++) {
        
        Object currentNote = objectList[i];
        
        // Is in treble clef.
        if (currentNote.pos.y < staffPosList[14]) {
            // Is onscreen.
            if (currentNote.pos.x > camLeft && currentNote.pos.x < camRight) {
                // Make sure next note exists.
                if (checkNextStreamNote(i) >= 0) {
                    Object nextNote = objectList[checkNextStreamNote(i)];
                    
                    // First, check if the next note is onscreen. // find me don't need this?
                    //if (nextNote.pos.x > camLeft && nextNote.pos.x < camRight) {
                    
                    // Now make sure the notes aren't already connected.
                    if (!(currentNote.bIsPartOfStream && nextNote.bIsPartOfStream)) {
                        
                        // Draw a line between the note centers to get the slope.
                        ofVec2f connection = nextNote.pos - currentNote.pos;
                        float myAngle = ofRadToDeg(atan2(connection.y, connection.x));
                        
                        // Now make sure the notes are close enough.
                        if (connection.lengthSquared() <= closeEnoughToTouch) {
                            
                            StreamBit tmp;
                            tmp.setup(currentNote.wide, currentNote.tall, currentNote.pos + (connection * 0.5), connection, 0, connection.length(), myAngle, currentNote.whichStream);
                            streamBitList.push_back(tmp);
                            
                            // Tell the actual notes not to connect again.
                            objectList[i].bIsPartOfStream = true;
                            objectList[checkNextStreamNote(i)].bIsPartOfStream = true;
                        }
                    }
                    //}
                }
                
                // Setup stream runoff.
                float angleOffset = PI / 4;
                float runoffLength = iScaler * 4;
                
                // Make stream "runoff" where the stream begins.
                if (!currentNote.bHasFalloffLeft && currentNote.bIsPartOfStream) {
                    
                    float distToPrevNote;
                    
                    // Check if there is another note, offscreen.
                    if (checkPrevStreamNote(i) == -2) {
                        
                        distToPrevNote = closeEnoughToTouch - 1;
                        
                    }
                    // Check if the note is the first one in the stream.
                    else if (checkPrevStreamNote(i) == -1) {
                        
                        distToPrevNote = closeEnoughToTouch + 1;
                        
                    } else {
                        
                        Object prevNote = objectList[checkPrevStreamNote(i)];
                        ofVec2f connection = currentNote.pos - prevNote.pos;
                        distToPrevNote = connection.lengthSquared();
                        
                    }
                    
                    if (distToPrevNote > closeEnoughToTouch) {
                        
                        float myAngle = checkNextStreamAngle(currentNote.pos) - ofRadToDeg(angleOffset);
                        ofVec2f endpoint = ofVec2f(currentNote.pos.x - cos(ofDegToRad(myAngle)) * runoffLength, currentNote.pos.y - sin(ofDegToRad(myAngle)) * runoffLength);
                        ofVec2f connection2 = currentNote.pos - endpoint;
                        float destroyOffset = abs(connection2.x * 0.5);
                        
                        StreamBit tmp;
                        tmp.setup(currentNote.wide, currentNote.tall, currentNote.pos - (connection2 * 0.5), connection2, destroyOffset, runoffLength, myAngle, currentNote.whichStream);
                        
                        streamBitList.push_back(tmp);
                        
                        // Tell the actual note not to repeat this.
                        objectList[i].bHasFalloffLeft = true;
                    }
                }
                
                // Make stream "runoff" where the stream ends.
                if (!currentNote.bHasFalloffRight && currentNote.bIsPartOfStream) {
                    
                    float distToNextNote;
                    
                    // Check if there is another note, offscreen.
                    if (checkNextStreamNote(i) == -2) {
                        
                        distToNextNote = closeEnoughToTouch - 1;
                        
                    }
                    // Check if the note is the last one in the stream.
                    else if (checkNextStreamNote(i) == -1) {
                        
                        distToNextNote = closeEnoughToTouch + 1;
                        
                    }
                    // Otherwise there is another note, onscreen.
                    else {
                        
                        Object nextNote = objectList[checkNextStreamNote(i)];
                        ofVec2f connection = nextNote.pos - currentNote.pos;
                        distToNextNote = connection.lengthSquared();
                        
                    }
                    
                    if (distToNextNote > closeEnoughToTouch) {
                        
                        float myAngle = checkPrevStreamAngle(currentNote.pos) + ofRadToDeg(angleOffset);
                        ofVec2f endpoint = ofVec2f(currentNote.pos.x + cos(ofDegToRad(myAngle)) * runoffLength, currentNote.pos.y + sin(ofDegToRad(myAngle)) * runoffLength);
                        ofVec2f connection2 = endpoint - currentNote.pos;
                        float destroyOffset = abs(connection2.x * 0.5);
                        
                        StreamBit tmp;
                        tmp.setup(currentNote.wide, currentNote.tall, currentNote.pos + (connection2 * 0.5), connection2, destroyOffset, runoffLength, myAngle, currentNote.whichStream);
                        streamBitList.push_back(tmp);
                        
                        // Tell the actual note not to repeat this.
                        objectList[i].bHasFalloffRight = true;
                        
                    }
                }
            }
        }
    }
    // Change the opacity on a timer and restart the timer.
    if (ofGetElapsedTimef() >= iLastOpacityChange + iOpacityChangeFreq) {
        for (int i = 0; i < streamBitList.size(); i++) {
            streamBitList[i].opacityState++;
        }
        iLastOpacityChange = ofGetElapsedTimef();
    }
    
    for (int i = 0; i < streamBitList.size(); i++) {
        
        // Set the opacity based on the last bit of the previous StreamBit.
        if (i > 0) {
            streamBitList[i].opacityState = streamBitList[i-1].opacityState - streamBitList[i - 1].numBits;
        }
        streamBitList[i].update();
        
        // Destroy offscreen streambits. They get created beyond the note so it's necessary to offset the destruction point.
        if (streamBitList[i].pos.x < camLeft - streamBitList[i].destroyOffset || streamBitList[i].pos.x > camRight + streamBitList[i].destroyOffset) {
            streamBitList[i].destroyMe = true;
        }
    }
    
    // Remove notes from the stream when they go offscreen.
    for (int i = 0; i < objectList.size(); i++) {
        if (objectList[i].pos.x < camLeft || objectList[i].pos.x > camRight) {
            objectList[i].bIsPartOfStream = false;
            objectList[i].bHasFalloffLeft = false;
            objectList[i].bHasFalloffRight = false;
        }
    }
}

//--------------------------------------------------------------
void testApp::playerCollidesWithGroundOrSky() {
    
    // P1
    
    float fGoNoLower;
    if ( !myPlayer.bModeSurf ) {
        fGoNoLower = iThirdOfScreen - myPlayer.tall / 2.0;
    } else if (gameState == 4) {
        fGoNoLower = iThirdOfScreen + iScaler * 7;
    } else {
        fGoNoLower = iThirdOfScreen + iScaler * 7 - myPlayer.tall;
    }
    if ( myPlayer.pos.y >= fGoNoLower ) {
        myPlayer.pos.y = fGoNoLower;
        if ( gameState < 3 || gameState == 4 ) {
            myPlayer.onSurface = true;
            if (bIsSecondPlayer) {
                myPlayer2.onSurface = true;
            }
        }
        if (gameState >= 7) {
            if (gameState == 8 && myPlayer.pos.x > streamBitList[streamBitList.size() -1].pos.x) {
                gameState = 7;
            }
            fLoadPrototype();
        }
    }
    
    float fGoNoHigher;
    if ( !myPlayer.bModeSurf ) {
        fGoNoHigher = myPlayer.tall / 2.0;
        
    } else {
        fGoNoHigher = -iScaler * 7;
    }
    if ( myPlayer.pos.y <= fGoNoHigher ) {
        myPlayer.pos.y = fGoNoHigher;
        myPlayer.vel.y = 0;
    }
    
    // P2
    
    if ( gameState >= 3 ) {
        if (bIsSecondPlayer) {
            float fGoNoLower2 = ofGetHeight() - iThirdOfScreen + myPlayer2.tall / 2.0;
            if ( myPlayer2.pos.y < fGoNoLower2 ) {
                myPlayer2.pos.y = fGoNoLower2;
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::playerCollidesWithObject() {
    
    // Collision with main objects vector.
    for ( int i = 0; i < objectList.size(); i++ ) {
        // Make some floats for shorthand...
        // Player is drawn from the center.
        // Obstacles are drawn from the corner.
        
        float margin = iScaler / 2.5;
        
        float playerTop = myPlayer.pos.y - myPlayer.tall / 2.0;
        float playerLeft = myPlayer.pos.x - myPlayer.wide / 2.0;
        float playerBottom;
        if (myPlayer.bHasShip) {
            // Bottom is center of ship, matching collision with stream.
            playerBottom = myPlayer.myShip.pos.y;
        } else {
            // Bottom is bottom of player.
            playerBottom = myPlayer.pos.y + myPlayer.tall / 2.0;
        }
        float playerRight = myPlayer.pos.x + myPlayer.wide / 2.0;
        
        float objectTop;
        if (myPlayer.bHasShip) {
            // Top is center of object, matching collision with stream.
            objectTop = objectList[ i ].pos.y;
        } else {
            // Top is top of object.
            objectTop = objectList[ i ].pos.y - objectList[ i ].tall / 2.0;
        }
        float objectLeft = objectList[ i ].pos.x - objectList[ i ].wide / 2.0;
        float objectBottom = objectList[ i ].pos.y + objectList[ i ].tall / 2.0;
        float objectRight = objectList[ i ].pos.x + objectList[ i ].wide / 2.0;
        
        // First, enable autoplay of bass if called.
        if (myPlayer.bAutoplayBass) {
            if (playerRight > objectLeft && playerLeft < objectRight) {
                if (objectList[i].pos.y >= staffPosList[14]) {
                    objectList[i].bIsTouched = true;
                }
            }
        }
        
        // Sound the note in proximity.
        if ( gameState < 3 ) {
            if ( playerRight >= objectLeft && playerLeft <= objectRight) {
                objectList[ i ].bIsTouched = true;
            }
        } else {
            
            //float fHealthMultiplier = 1.5;
            float surfOffset;
            if (myPlayer.bHasShip) {
                surfOffset = objectList[i].tall / 2.0 + myPlayer.myShip.fImgHeightBass * 0.5;
            } else {
                surfOffset = 0;
            }
            if ( playerRight >= objectLeft && playerLeft <= objectRight
                && playerBottom >= (objectTop - surfOffset) && playerTop <= objectBottom ) {
                
                //myPlayer.fHealth += myPlayer.fHealthLossSpeed * fHealthMultiplier;
                objectList[ i ].bIsTouched = true;
            }
            
            if (bIsSecondPlayer) {
                float player2Top = myPlayer2.pos.y - myPlayer2.tall / 2.0;
                float player2Left = myPlayer2.pos.x - myPlayer2.wide / 2.0;
                float player2Bottom = myPlayer2.pos.y + myPlayer2.tall / 2.0;
                float player2Right = myPlayer2.pos.x + myPlayer2.wide / 2.0;
                
                if ( player2Right > objectLeft && player2Left < objectRight
                    && player2Bottom > objectTop && player2Top < objectBottom ) {
                    
                    //myPlayer2.fHealth += myPlayer2.fHealthLossSpeed * fHealthMultiplier;
                    objectList[ i ].bIsTouched = true;
                }
            }
        }
        
        // Prevent the player from moving through the note.
        if ((myPlayer.bModePlatformer /*|| myPlayer.bModeSurf*/) && gameState != 4) {
            
            // Check if in the same horizontal region as the object.
            if ( playerBottom >= objectTop + margin && playerTop <= objectBottom - margin ) {
                
                // Is there something directly to the right?
                if ( playerRight >= objectLeft && playerRight < objectList[ i ].pos.x ) {
                    // Prevent the player from moving to the right.
                    myPlayer.pos.x = objectLeft - myPlayer.wide / 2.0;
                }
                
                // OK, is there something directly to the left?
                else if ( playerLeft <= objectRight && playerLeft > objectList[ i ].pos.x ) {
                    // Prevent the player from moving to the left.
                    myPlayer.pos.x = objectRight + myPlayer.wide / 2.0;
                }
            }
            
            // Next, check if the player is in the same vertical region as the object.
            if ( playerRight >= objectLeft + margin && playerLeft <= objectRight - margin ) {
                
                // Is there something directly below?
                if ( playerBottom >= objectTop && playerBottom < objectList[ i ].pos.y ) {
                    // Prevent the player from moving downward.
                    if (myPlayer.bHasShip) {
                        myPlayer.pos.y = objectTop - myPlayer.myShip.fImgHeightBass / 2.0 - (myPlayer.myShip.pos.y - myPlayer.pos.y);
                    } else {
                        myPlayer.pos.y = objectTop - myPlayer.tall / 2.0;
                    }
                    myPlayer.onSurface = true;
                    myPlayer.applyForce( objectList[ i ].vel );
                }
                
                // OK, is there something directly above?
                else if ( playerTop <= objectBottom && playerTop > objectList[ i ].pos.y ) {
                    // Prevent the player from moving upward.
                    myPlayer.pos.y = objectBottom + myPlayer.tall / 2.0;
                    // Cancel any upward velocity.
                    if ( myPlayer.vel.y < 0 ) {
                        myPlayer.vel.y = 0;
                    }
                }
            }
        }
        
        
        /*if ( myPlayer.pos.x == objectList[ i ].pos.x && myPlayer.pos.y == objectList[ i ].pos.y ) {
         myPlayer.applyForce( ofVec2f( -10.0, 0.0 ) );
         }*/
    }
}

//--------------------------------------------------------------
void testApp::playerCollidesWithStream() {
    
    for (int i = 0; i < streamBitList.size(); i++) {
        
        ofVec2f start = streamBitList[i].pos - streamBitList[i].slope * 0.5;
        ofVec2f end = streamBitList[i].pos + streamBitList[i].slope * 0.5;
        ofVec2f inc = streamBitList[i].slope.normalized();
        float diff = myPlayer.myShip.pos.y - myPlayer.pos.y /*+ streamBitList[i].wide * 0.5*/;
        for (int j = 1; j < int(streamBitList[i].wide); j++) {
            
            // If player x is within two bits of the object's x...
            if (myPlayer.pos.x >= start.x + inc.x * (j - 1) && myPlayer.pos.x <= start.x + inc.x * j) {
                
                // Snap to descending stream if already riding it and not jumping.
                if (!myPlayer.onStream && myPlayer.inStreamTimer > 0 && myPlayer.currentStream == streamBitList[i].whichStream) {
                    if (myPlayer.pos.y < start.y + inc.y * j - diff) {
                        myPlayer.pos.y = start.y + inc.y * j - diff;
                    }
                }
                
                // Find me--delete all of this if find better solution to hiding the issue.
                {// Make the ship land correctly in the stream.
                    ofVec2f otherPoint;
                    float hyp = myPlayer.myShip.rotOffset * 2;
                    if (myPlayer.myShip.rotPoint == -1) {
                        otherPoint.set(myPlayer.myShip.pos.x + hyp * (cos(ofDegToRad(myPlayer.myShip.angle))), myPlayer.myShip.pos.y + hyp * (sin(ofDegToRad(myPlayer.myShip.angle))));
                    } else if (myPlayer.myShip.rotPoint == 1) {
                        otherPoint.set(myPlayer.myShip.pos.x - hyp * (cos(ofDegToRad(myPlayer.myShip.angle))), myPlayer.myShip.pos.y - hyp * (sin(ofDegToRad(myPlayer.myShip.angle))));
                    }
                    
                    if (otherPoint.y >= start.y + inc.y && myPlayer.myShip.pos.y < start.y + inc.y) {
                        ofVec2f diff, refPoint, dest;
                        if (myPlayer.myShip.rotPoint == -1) {
                            diff = myPlayer.pos - myPlayer.myShip.pointFront;
                            refPoint = myPlayer.myShip.pointRear;
                            dest = refPoint + diff;
                        } else if (myPlayer.myShip.rotPoint == 1) {
                            diff = myPlayer.pos - myPlayer.myShip.pointRear;
                            refPoint = myPlayer.myShip.pointFront;
                            dest = refPoint + diff;
                        }
                        //myPlayer.pos = otherPoint + diff;
                        //cout<<otherPoint<<" "<<myPlayer.pos<<endl;
                        //myPlayer.myShip.rotPoint *= -1;
                        //updateGame = false;
                        //myPlayer.pos.y -= 200;
                    }
                } // End delete
                
                // Keep player on top of stream, if not underneath stream.
                if (myPlayer.pos.y >= start.y + inc.y * j - diff
                    && myPlayer.myShip.pos.y <= start.y + inc.y * j + streamBitList[i].tall * 0.5) {
                    myPlayer.pos.y = start.y + inc.y * j - diff;
                    if (myPlayer.vel.y >= 0) { // Make sure not jumping.
                        myPlayer.onStream = true;
                        myPlayer.currentStream = streamBitList[i].whichStream;
                    }
                    
                    /*
                     Player must align the board to the stream to surf at full speed. Check this by comparing the board's angle to the stream's. However sometimes the player may be coming off a sharp change in angle and not be able to adjust quickly enough. So we take into account the previous and next angles, within a limited region.
                     */
                    // FIND ME, TUNE ME, TUNE ME SO MUCH.
                    // Check the angle and adjust speed accordingly.
                    int closeEnough = 15; // How close the two angles have to be.
                    int otherBitProximity = iScaler; // How close the other Bit's end has to be.
                    int checkOtherBitMultiplier = otherBitProximity; // How many intervals of the current bit check against the other Bits.
                    float angleDiff = abs(myPlayer.myShip.angle - streamBitList[i].angle);
                    
                    if (angleDiff > 180) {
                        angleDiff = 360 - angleDiff;
                    }
                    
                    if (angleDiff <= closeEnough) {
                        myPlayer.closeEnough = true;
                    }
                    
                    // If not close enough, check the previous and next angles if they exist.
                    else {
                        // Autorotate if leaning too far. Find me
                        //myPlayer.myShip.angle += myPlayer.myShip.rotPoint * 0.5;
                        // Previous angle.
                        if (i > 0) {
                            
                            ofVec2f headOfBit = start + inc * checkOtherBitMultiplier;
                            ofVec2f endPrev = streamBitList[i - 1].pos + streamBitList[i - 1].slope * 0.5;
                            float angleDiffPrev = abs(myPlayer.myShip.angle - streamBitList[i - 1].angle);
                            
                            if (angleDiffPrev > 180) {
                                angleDiffPrev = 360 - angleDiffPrev;
                            }
                            
                            // Make sure the prev Bit is close enough too.
                            if (angleDiffPrev <= closeEnough && myPlayer.pos.x < headOfBit.x && abs(start.x - endPrev.x) < otherBitProximity) {
                                myPlayer.closeEnough = true;
                            }
                        }
                        // Upcoming angle.
                        if (i < streamBitList.size() - 1) {
                            
                            ofVec2f tailOfBit = end - inc * checkOtherBitMultiplier;
                            ofVec2f startNext = streamBitList[i + 1].pos - streamBitList[i + 1].slope * 0.5;
                            float angleDiffNext = abs(myPlayer.myShip.angle - streamBitList[i + 1].angle);
                            
                            if (angleDiffNext > 180) {
                                angleDiffNext = 360 - angleDiffNext;
                            }
                            
                            // Make sure the next Bit is close enough too.
                            if (angleDiffNext <= closeEnough && myPlayer.pos.x > tailOfBit.x && abs(startNext.x - end.x) < otherBitProximity) {
                                myPlayer.closeEnough = true;
                            }
                        }
                    }
                }
                // Bonk off bottom of stream, if not above stream and not in stream.
                if (myPlayer.pos.y - myPlayer.tall * 0.5 <= start.y + inc.y * j + streamBitList[i].tall * 0.5
                    && myPlayer.pos.y >= start.y + inc.y * j - streamBitList[i].tall * 0.5
                    && !myPlayer.onStream) {
                    myPlayer.pos.y = start.y + inc.y * j + streamBitList[i].tall * 0.5 + myPlayer.tall * 0.5;
                    if (myPlayer.vel.y < 0) {
                        myPlayer.vel.y = 0;
                    }
                }
            }
        }
    }
    
    // Fall over if angled too sharply and not in stream. Find me
    if (myPlayer.onStream && myPlayer.myShip.angle > 90 && myPlayer.myShip.angle < 270) {
        //bPlayerFellOver = true;
        //myPlayer.allowControl = false;
    }
}

//--------------------------------------------------------------
void testApp::playerCollidesWithObstacle() {
    
    // Collision with Obstacle vector.
    for ( int i = 0; i < obstacleList.size(); i++ ) {
        
        // Make some floats for shorthand...
        // Player is drawn from the center.
        // Obstacles are drawn from the corner.
        float margin = iScaler / 2.5;
        float playerTop = myPlayer.pos.y - myPlayer.tall / 2.0;
        float playerLeft = myPlayer.pos.x - myPlayer.wide / 2.0;
        float playerBottom = myPlayer.pos.y + myPlayer.tall / 2.0;
        float playerRight = myPlayer.pos.x + myPlayer.wide / 2.0;
        float obstacleTop = obstacleList[ i ].pos.y;
        float obstacleLeft = obstacleList[ i ].pos.x;
        float obstacleBottom = obstacleList[ i ].pos.y + obstacleList[ i ].tall;
        float obstacleRight = obstacleList[ i ].pos.x + obstacleList[ i ].wide;
        
        // First, check if the player is in the same horizontal region as the obstacle.
        if ( playerBottom >= obstacleTop + margin && playerTop <= obstacleBottom - margin ) {
            // Is there something directly to the right?
            if ( playerRight >= obstacleLeft && playerRight < obstacleList[ i ].pos.x + obstacleList[ i ].wide / 2.0 ) {
                // Prevent the player from moving to the right.
                myPlayer.pos.x = obstacleLeft - myPlayer.wide / 2.0;
            }
            // OK, is there something directly to the left?
            else if ( playerLeft <= obstacleRight && playerLeft > obstacleList[ i ].pos.x + obstacleList[ i ].wide / 2.0 ) {
                // Prevent the player from moving to the left.
                myPlayer.pos.x = obstacleRight + myPlayer.wide / 2.0;
            }
        }
        // Next, check if the player is in the same vertical region as the obstacle.
        if ( playerRight >= obstacleLeft + margin && playerLeft <= obstacleRight - margin ) {
            // Is there something directly below?
            if ( playerBottom >= obstacleTop && playerBottom < obstacleList[ i ].pos.y + obstacleList[ i ].tall / 2.0 ) {
                // Prevent the player from moving downward.
                myPlayer.pos.y = obstacleTop - myPlayer.tall / 2.0;
                myPlayer.onSurface = true;
                //                myPlayer.vel.set( objectList[ i ].vel );
                //myPlayer.applyForce( objectList[ i ].vel );
            }
            // OK, is there something directly above?
            else if ( playerTop <= obstacleBottom && playerTop > obstacleList[ i ].pos.y + obstacleList[ i ].tall / 2.0 ) {
                // Prevent the player from moving upward.
                myPlayer.pos.y = obstacleBottom + myPlayer.tall / 2.0;
                // Cancel any upward velocity.
                if ( myPlayer.vel.y < 0 ) {
                    myPlayer.vel.y = 0;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::objectCollidesWithObstacle() {
    
    // Collision with Obstacle vector.
    for ( int i = 0; i < objectList.size(); i++ ) {
        for ( int j = 0; j < obstacleList.size(); j++ ) {
            
            // Make some floats for shorthand...
            // Object is drawn from the center.
            // Obstacles are drawn from the corner.
            float margin = 0.0;
            float objectTop = objectList[ i ].pos.y - objectList[ i ].tall / 2.0;
            float objectLeft = objectList[ i ].pos.x - objectList[ i ].wide / 2.0;
            float objectBottom = objectList[ i ].pos.y + objectList[ i ].tall / 2.0;
            float objectRight = objectList[ i ].pos.x + objectList[ i ].wide / 2.0;
            float obstacleTop = obstacleList[ j ].pos.y;
            float obstacleLeft = obstacleList[ j ].pos.x;
            float obstacleBottom = obstacleList[ j ].pos.y + obstacleList[ j ].tall;
            float obstacleRight = obstacleList[ j ].pos.x + obstacleList[ j ].wide;
            
            // First, check if the object is in the same horizontal region as the obstacle.
            if ( objectBottom >= obstacleTop + margin && objectTop <= obstacleBottom - margin ) {
                // Is there something directly to the right?
                if ( objectRight >= obstacleLeft && objectRight < obstacleList[ j ].pos.x + obstacleList[ j ].wide / 2.0 ) {
                    // Reverse velocity.
                    objectList[ i ].vel.x *= -1;
                }
                // OK, is there something directly to the left?
                else if ( objectLeft <= obstacleRight && objectLeft > obstacleList[ j ].pos.x + obstacleList[ j ].wide / 2.0 ) {
                    // Reverse velocity.
                    objectList[ i ].vel.x *= -1;
                }
            }
            // Next, check if the object is in the same vertical region as the obstacle.
            if ( objectRight >= obstacleLeft + margin && objectLeft <= obstacleRight - margin ) {
                // Is there something directly below?
                if ( objectBottom >= obstacleTop && objectBottom < obstacleList[ j ].pos.y + obstacleList[ j ].tall / 2.0 ) {
                    // Reverse velocity.
                    objectList[ i ].vel.y *= -1;
                }
                // OK, is there something directly above?
                else if ( objectTop <= obstacleBottom && objectTop > obstacleList[ j ].pos.y + obstacleList[ j ].tall / 2.0 ) {
                    // Reverse velocity.
                    objectList[ i ].vel.y *= -1;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::fDrawRestartScreen() {
    
    ofSetColor( 0 );
    helvetica.drawString( "Are you sure you want to restart? Y / N ", ofGetWidth() / 2 - iScaler * 13, ofGetHeight() / 2.0 );
}

//--------------------------------------------------------------
void testApp::fDrawDebugUI() {
    
    ofSetColor( 0 );
    if ( bIsDebugging ) {
        for (int i = 0; i < streamBitList.size(); i++ ) {
            streamBitList[i].bDrawCollider = true;
        }
        helvetica.drawString( "FPS: " + ofToString( ofGetFrameRate() ), myPlayer.pos.x - ofGetWidth() / 2, iScaler * 2 );
        helvetica.drawString( "Debug mode ON ( '0' to turn OFF )", myPlayer.pos.x - iScaler * 12, iScaler * 2 );
    } else {
        for (int i = 0; i < streamBitList.size(); i++ ) {
            streamBitList[i].bDrawCollider = false;
        }
    }
    if ( myPlayer.pos.x > ofGetWidth() / 2 && gameState >= 3 && gameState < 6 ) {
        helvetica.drawString( "'R' to restart", myPlayer.pos.x + ofGetWidth() / 2 - iScaler * 8.4, iScaler * 2 );
    }
}

//--------------------------------------------------------------
void testApp::fDrawGround() {
    
    // Draw some lines on the ground to give something to move against.
    if ( gameState < 3 ) {
        ofSetColor( 0 );
        for ( int i = 0; i < iScaler * 800; i += iScaler * 2 ) {
            ofLine( iScaler * 2 + ( i * 2 ), iThirdOfScreen + iScaler, iScaler * 2 + i, iThirdOfScreen );
        }
    } else if ( gameState == 4 ) {
        ofSetColor( 0 );
        for ( int i = 0; i < iScaler * 800; i += iScaler * 2 ) {
            ofLine( iScaler * 2 + ( i * 2 ), iThirdOfScreen + iScaler * 7, iScaler * 2 + i, iThirdOfScreen + iScaler * 8 );
        }
    }
}

//--------------------------------------------------------------
void testApp::fCalcAllNotePos() {
    
    for ( int i = 0; i < numYpos; i++ ) {
        float tmp;
        if ( i < 15 ) { // Bottom clef.
            tmp = ofGetHeight() - ( iScaler * ( i + 1 ) );
        } else { // Top clef.
            tmp = ofGetHeight() - ( ofGetHeight() - iThirdOfScreen * 2 ) - ( iScaler * ( i + 2 ) );
        }
        staffPosList.push_back( tmp );
    }
}

//--------------------------------------------------------------
void testApp::fCalcTrebleNotes() {
    
    iTotalTrebleNotes = 0; // Reset this every frame.
    for ( int i = 0; i < objectList.size(); i++ ) {
        if ( objectList[ i ].pos.y <= iThirdOfScreen ) {
            iTotalTrebleNotes++;
        } else {
            objectList[ i ].bWasTouched = false;
        }
    }
}

//--------------------------------------------------------------
string testApp::fReturnNote( float yPos ) {
    
    float _yPos;
    
    // Calculate which staff position is closest to the player's position.
    // Look from the staff position half up and half down to the next positions.
    for ( int i = 0; i < staffPosList.size(); i++ ) {
        if ( yPos > staffPosList[ i ] - ( staffPosList[ i ] - staffPosList[ i + 1 ] ) * 0.5
            && yPos <= staffPosList[ i ] + ( staffPosList[ i - 1 ] - staffPosList[ i ] ) * 0.5 ) {
            _yPos = staffPosList[ i ];
        }
    }
    
    string tmp;
    
    if ( _yPos == staffPosList[ 0 ] ) {
        tmp = "d1";
        //tmp = "d1#"
    }
    else if ( _yPos == staffPosList[ 1 ] ) {
        tmp = "e1";
    }
    else if ( _yPos == staffPosList[ 2 ] ) {
        tmp = "f1";
        //tmp = "f1#"
    }
    else if ( _yPos == staffPosList[ 3 ] ) {
        tmp = "g1";
        //tmp = "g1#"
    }
    else if ( _yPos == staffPosList[ 4 ] ) {
        tmp = "a1";
        //tmp = "a1#"
    }
    else if ( _yPos == staffPosList[ 5 ] ) {
        tmp = "b1";
    }
    else if ( _yPos == staffPosList[ 6 ] ) {
        tmp = "c2";
        //tmp = "c2#"
    }
    else if ( _yPos == staffPosList[ 7 ] ) {
        tmp = "d2";
        //tmp = "d2#"
    }
    else if ( _yPos == staffPosList[ 8 ] ) {
        tmp = "e2";
    }
    else if ( _yPos == staffPosList[ 9 ] ) {
        tmp = "f2";
        //tmp = "f2#"
    }
    else if ( _yPos == staffPosList[ 10 ] ) {
        tmp = "g2";
        //tmp = "g2#"
    }
    else if ( _yPos == staffPosList[ 11 ] ) {
        tmp = "a2";
        //tmp = "a2#"
    }
    else if ( _yPos == staffPosList[ 12 ] ) {
        tmp = "b2";
    }
    // 13 = bass c; 14 = bass d; 15 = treble b
    else if ( _yPos == staffPosList[ 16 ] ) {
        tmp = "c3_middle";
        //tmp = "c3#"
    }
    else if ( _yPos == staffPosList[ 17 ] ) {
        tmp = "d3";
        //tmp = "d3#"
    }
    else if ( _yPos == staffPosList[ 18 ] ) {
        tmp = "e3";
    }
    else if ( _yPos == staffPosList[ 19 ] ) {
        tmp = "f3";
        //tmp = "f3#"
    }
    else if ( _yPos == staffPosList[ 20 ] ) {
        tmp = "g3";
        //tmp = "g3#"
    }
    else if ( _yPos == staffPosList[ 21 ] ) {
        tmp = "a3";
        //tmp = "a3#"
    }
    else if ( _yPos == staffPosList[ 22 ] ) {
        tmp = "b3";
    }
    else if ( _yPos == staffPosList[ 23 ] ) {
        tmp = "c4";
        //tmp = "c4#"
    }
    else if ( _yPos == staffPosList[ 24 ] ) {
        tmp = "d4";
        //tmp = "d4#"
    }
    else if ( _yPos == staffPosList[ 25 ] ) {
        tmp = "e4";
    }
    else if ( _yPos == staffPosList[ 26 ] ) {
        tmp = "f4";
        //tmp = "f4#"
    }
    else if ( _yPos == staffPosList[ 27 ] ) {
        tmp = "g4";
        //tmp = "g4#"
    }
    else if ( _yPos == staffPosList[ 28 ] ) {
        tmp = "a4";
        //tmp = "a4#"
    }
    else if ( _yPos == staffPosList[ 29 ] ) {
        tmp = "b4";
    }
    
    return tmp;
}

//--------------------------------------------------------------
void testApp::addObject( string _note, float _xPos, int _stream, int _age ) {
    
    // This function adds an NPC Object.
    Object tmp;
    tmp.setup( iScaler, staffPosList, _note, _stream, _age );
    tmp.pos.x = _xPos;
    objectList.push_back( tmp );
}

void testApp::addObject( vector < string > _stringList, int _numMeasures, int _numReps ) {
    for (int j = 0; j < _numReps; j++ ) {
        for ( int i = 0; i < _stringList.size(); i += 4 ) {
            Object tmp;
            tmp.setup( iScaler, staffPosList, _stringList[ i ], ofToInt( _stringList[ i + 2 ] ), ofToInt( _stringList[ i + 3 ] ) );
            tmp.pos.x = ofToFloat( _stringList[ i + 1  ] ) + (j * fMeasureLength * _numMeasures);
            objectList.push_back( tmp );
        }
    }
}

//--------------------------------------------------------------
void testApp::addRecordedObject( string _note, ofVec2f _vel, int _stream, int _age ) {
    
    // This function copies a recorded Object into a static vector that gets neither updated nor drawn.
    Object tmp;
    tmp.setup( iScaler, staffPosList, _note, _stream, _age );
    tmp.vel.set( _vel );
    recordedList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::addReplayedObject( string _note, ofVec2f _vel, int _stream, int _age ) {
    
    // This function copies an Object from the "recorded" vector to the main Object vector. It also reverses horizontal velocity if needed so the Object can travel the other direction.
    Object tmp;
    tmp.setup( iScaler, staffPosList, _note, _stream, _age );
    tmp.pos.x = myPlayer.pos.x;
    tmp.vel.set( _vel );
    if ( tmp.vel.x < 0 ) tmp.vel.x *= -1;
    objectList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::fRecord( int _i ) {
    
    if ( bIsRecording || recordedList.size() == myPlayer.capacity ) return;
    
    // Set the color to be different to indicate recording.
    objectList[ _i ].bIsRecorded = true;
    
    // Check the spacing between the recorded note and the previous note.
    /*float xDist;
     if ( recordedList.size() == 0 ) {
     xDist = 0;
     } else { // I may need to adjust the below in case notes aren't captured linearly.
     xDist = objectList[ i ].pos.x - objectList[ i - 1 ].pos.x;
     }*/
    addRecordedObject( objectList[ _i ].whichNote, objectList[ _i ].vel, objectList[ _i ].whichStream, objectLife );
    
    // This prevents additional recording calls before the action completes.
    bIsRecording = true;
    
    if ( bHighlightNote && _i != getThisOne ) {
        getThisOne = 0;
    }
}

//--------------------------------------------------------------
void testApp::fReplay() {
    
    // Replay mode pauses movement.
    
    // Don't proceed if there are zero recorded notes.
    if ( recordedList.size() == 0 ) {
        return;
    }
    
    myPlayer.allowMove = false;
    
    /*float xDist;
     if ( replayedList.size() > 0 ) {
     xDist = replayedList[ replayedList.size() - 1 ].pos.x - myPlayer.pos.x;
     } else {
     xDist = 0;
     }*/
    
    //if ( xDist >= recordedList[ 0 ].spacing ) {
    addReplayedObject( recordedList[ 0 ].whichNote, recordedList[ 0 ].vel, recordedList[ 0 ].whichStream, recordedList[ 0 ].age );
    recordedList[ 0 ].destroyMe = true;
    //}
    
    myPlayer.allowMove = true;
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
        for ( int j = 0; j < objectList[ i ].noteList.size(); j++ ) {
            objectList[ i ].fCleanup();
        }
    }
    for ( int i = 0; i < recordedList.size(); i++ ) {
        for ( int j = 0; j < recordedList[ i ].noteList.size(); j++ ) {
            recordedList[ i ].fCleanup();
        }
    }
    
    // Clear the vector.
    obstacleList.clear();
    objectList.clear();
    recordedList.clear();
    streamBitList.clear();
}

/*
 //--------------------------------------------------------------
 void testApp::mouseMoved(int x, int y ){
 
 }
 
 //--------------------------------------------------------------
 void testApp::mouseDragged(int x, int y, int button){
 
 }
 
 //--------------------------------------------------------------
 void testApp::mousePressed(int x, int y, int button){
 
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
 */