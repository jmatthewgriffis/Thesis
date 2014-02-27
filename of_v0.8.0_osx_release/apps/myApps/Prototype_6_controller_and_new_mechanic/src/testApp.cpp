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
    
    trebleClef.loadImage( "images/clef_treble.png" ); // http://clipartist.info/SVG/CLIPARTIST.ORG/TREBLE/treble_clef_treble_clef-555px.png
    bassClef.loadImage( "images/clef_bass.png" ); // http://clipartist.info/RSS/openclipart.org/2011/April/15-Friday/bass_clef_bassclef-1979px.png
    staffBracket.loadImage( "images/staff_bracket.png" ); // http://musescore.org/sites/musescore.org/files/issues/Curly%20brace%20extends%20to%20all%20staves.png
    
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
        //cout<<iScaler<<endl;
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
     */
    gameState = 0;
    currentState = gameState;
    
    bIsLefty = bIsRecording = bIsDebugging = bShiftIsPressed = myTitle.bChoseControls = false;
    bHighlightNote = false;
    bCamZoomedIn = false;
    
    if ( bHighlightNote ) getThisOne = 0;
    else getThisOne = -1;
    objectLife = 7 * frameRate;
    fMeasureLength = iScaler * 6 * 5.333333;;
    iStaffAlphaMin = 10;
    iStaffAlphaMax = 200;
    iStaffAlpha = iStaffAlphaMin;
    iStaffAlphaVel = 0.5;
    myTitle.iWhichPrototype = 1;
}

//--------------------------------------------------------------
bool bShouldIErase( Object &a ){
    
    if ( a.destroyMe ) return true;
    else return false;
}

//--------------------------------------------------------------
void testApp::update(){
    
    { // Straight from the ofxGamepad example.
        //CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
        /*if(ofxGamepadHandler::get()->getNumPads()>0){
         ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
         ofAddListener(pad->onAxisChanged, this, &testApp::axisChanged);
         ofAddListener(pad->onButtonPressed, this, &testApp::buttonPressed);
         ofAddListener(pad->onButtonReleased, this, &testApp::buttonReleased);
         }*/
    } // End ofxGamepad stuff
    
    // Don't update anything else if not on the game screen.
    if ( gameState < 1 ) return;
    
    // Update the alpha for the staff lines.
    iStaffAlpha += iStaffAlphaVel;
    if ( iStaffAlpha < iStaffAlphaMin || iStaffAlpha > iStaffAlphaMax ) {
        iStaffAlphaVel *= -1;
    }
    
    // Reset essential conditionals.
    myPlayer.onSurface = false;
    
    if ( bHighlightNote ) {
        if ( getThisOne < 0 ) getThisOne = objectList.size() - 1;
        if ( getThisOne > objectList.size() - 1 ) getThisOne = 0;
    }
    
    // Apply gravity to the player.
    // Come back to this. Use it to fake analog sensitivity with jump height proportional to how long the button is held. Gravity only applies sometimes.
    if ( gameState != 3 ) {
        myPlayer.applyForce( ofVec2f( 0.0, iScaler * 0.012 ) );
    }
    
    // Run collision detection.
    playerCollidesWithGround();
    playerCollidesWithObstacle();
    playerCollidesWithObject();
    objectCollidesWithObstacle();
    
    // Update the player (duh).
    myPlayer.update( gameState );
    if ( gameState >= 3 ) {
        myPlayer2.update( gameState );
    }
    
    // Update the notes.
    updateObjectList();
    
    ofRemove( objectList, bShouldIErase );
    ofRemove( recordedList, bShouldIErase );
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //ofxGamepadHandler::get()->draw(10,10);
    
    if ( gameState == 0 ) {
        myTitle.draw( iScaler, helvetica, bIsLefty );
    }
    
    else if ( gameState == -1 ) {
        fDrawRestartScreen();
    }
    
    // Don't draw anything else if not on the game screen.
    else if ( gameState > 0 ) {
        myCam.begin();
        myCam.setupPerspective();
        
        ofSetColor( 0 );
        
        // LOCATION-INDEPENDENT
        
        //( ofGetHeight() - iScaler * 8 ) * staffBracket.getWidth() / staffBracket.getHeight() (staffStart)
        
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
        if ( fMoveX < 0 || gameState == 2 ) {
            myCam.move( 0, fMoveY, fZoomFactor );
        } else if ( fMoveX >= 0 ) {
            myCam.move( fMoveX, fMoveY, fZoomFactor );
        }
        
        ofSetColor( 0 );
        if ( bIsDebugging ) {
            helvetica.drawString( "FPS: " + ofToString( ofGetFrameRate() ), myPlayer.pos.x - ofGetWidth() / 2, iScaler * 2 );
            helvetica.drawString( "Debug mode ON ( '0' to turn OFF )", myPlayer.pos.x - iScaler * 12, iScaler * 2 );
        }
        if ( myPlayer.pos.x > ofGetWidth() / 2 && gameState >= 3 ) {
            helvetica.drawString( "'R' to restart", myPlayer.pos.x + ofGetWidth() / 2 - iScaler * 8.4, iScaler * 2 );
        }
        
        fDrawStaff();
        
        // Draw some lines on the ground to give something to move against.
        if ( gameState < 3 ) {
            ofSetColor( 0 );
            for ( int i = 0; i < iScaler * 800; i += iScaler * 2 ) {
                ofLine( iScaler * 2 + ( i * 2 ), iThirdOfScreen + iScaler, iScaler * 2 + i, iThirdOfScreen );
            }
        }
        
        // LOCATION-DEPENDENT
        
        if ( gameState == 1 ) {
            myTutorial.draw( helvetica );
        } else if ( gameState == 2 ) {
            myBoss.draw( helvetica );
        }
        
        // Draw the notes.
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].draw();
        }
        
        // Draw the obstacles.
        for ( int i = 0; i < obstacleList.size(); i++ ) {
            obstacleList[ i ].draw();
        }
        
        myPlayer.draw( helvetica, recordedList );
        if ( gameState >= 3 ) {
            myPlayer2.draw( helvetica, recordedList );
        }
        
        
        myCam.end();
    }
}

//--------------------------------------------------------------
void testApp::addObject( string _note, float _xPos, int _age ) {
    
    // This function adds an NPC Object.
    Object tmp;
    tmp.setup( iScaler, staffPosList, _note, _age );
    tmp.pos.x = _xPos;
    objectList.push_back( tmp );
}

void testApp::addObject( vector < string > _stringList ) {
    
    for ( int i = 0; i < _stringList.size(); i += 3 ) {
        // This function adds an NPC Object.
        Object tmp;
        tmp.setup( iScaler, staffPosList, _stringList[ i ], ofToInt( _stringList[ i + 2 ] ) );
        tmp.pos.x = ofToFloat( _stringList[ i + 1  ] );
        objectList.push_back( tmp );
    }
}

//--------------------------------------------------------------
void testApp::addRecordedObject( string _note, ofVec2f _vel, int _age ) {
    
    // This function copies a recorded Object into a static vector that gets neither updated nor drawn.
    Object tmp;
    tmp.setup( iScaler, staffPosList, _note, _age );
    tmp.vel.set( _vel );
    recordedList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::addReplayedObject( string _note, ofVec2f _vel, int _age ) {
    
    // This function copies an Object from the "recorded" vector to the main Object vector. It also reverses horizontal velocity if needed so the Object can travel the other direction.
    Object tmp;
    tmp.setup( iScaler, staffPosList, _note, _age );
    tmp.pos.x = myPlayer.pos.x;
    tmp.vel.set( _vel );
    if ( tmp.vel.x < 0 ) tmp.vel.x *= -1;
    objectList.push_back( tmp );
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
    addRecordedObject( objectList[ _i ].whichNote, objectList[ _i ].vel, objectLife );
    
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
    addReplayedObject( recordedList[ 0 ].whichNote, recordedList[ 0 ].vel, recordedList[ 0 ].age );
    recordedList[ 0 ].destroyMe = true;
    //}
    
    myPlayer.allowMove = true;
}

//--------------------------------------------------------------
void testApp::playerCollidesWithGround() {
    
    if ( myPlayer.pos.y >= iThirdOfScreen - myPlayer.tall / 2.0 ) {
        myPlayer.pos.y = iThirdOfScreen - myPlayer.tall / 2.0;
        if ( gameState < 3 ) {
            myPlayer.onSurface = true;
        }
    }
    
    if ( gameState >= 3 ) {
        if ( myPlayer2.pos.y < ofGetHeight() - iThirdOfScreen + myPlayer.tall / 2.0 ) {
            myPlayer2.pos.y = ofGetHeight() - iThirdOfScreen + myPlayer.tall / 2.0;
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
        float playerBottom = myPlayer.pos.y + myPlayer.tall / 2.0;
        float playerRight = myPlayer.pos.x + myPlayer.wide / 2.0;
        float objectTop = objectList[ i ].pos.y - objectList[ i ].tall / 2.0;
        float objectLeft = objectList[ i ].pos.x - objectList[ i ].wide / 2.0;
        float objectBottom = objectList[ i ].pos.y + objectList[ i ].tall / 2.0;
        float objectRight = objectList[ i ].pos.x + objectList[ i ].wide / 2.0;
        
        float player2Top = myPlayer2.pos.y - myPlayer2.tall / 2.0;
        float player2Left = myPlayer2.pos.x - myPlayer2.wide / 2.0;
        float player2Bottom = myPlayer2.pos.y + myPlayer2.tall / 2.0;
        float player2Right = myPlayer2.pos.x + myPlayer2.wide / 2.0;
        
        if ( gameState >= 3 ) {
            float fHealthMultiplier = 1.5;
            if ( playerRight > objectLeft
                && playerLeft < objectRight
                && playerBottom > objectTop
                && playerTop < objectBottom ) {
                myPlayer.fHealth += myPlayer.fHealthLossSpeed * fHealthMultiplier;
                objectList[ i ].bIsTouched = true;
            }
            if ( player2Right > objectLeft
                && player2Left < objectRight
                && player2Bottom > objectTop
                && player2Top < objectBottom ) {
                myPlayer2.fHealth += myPlayer2.fHealthLossSpeed * fHealthMultiplier;
                objectList[ i ].bIsTouched = true;
            }
        }
        
        if ( gameState < 3 ) {
            // First, check if the player is in the same horizontal region as the object.
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
                
                objectList[ i ].bIsTouched = true;
                
                // Is there something directly below?
                if ( playerBottom >= objectTop && playerBottom < objectList[ i ].pos.y ) {
                    // Prevent the player from moving downward.
                    myPlayer.pos.y = objectTop - myPlayer.tall / 2.0;
                    myPlayer.onSurface = true;
                    //                myPlayer.vel.set( objectList[ i ].vel );
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
}

//--------------------------------------------------------------
void testApp::fDrawRestartScreen() {
    
    ofSetColor( 0 );
    helvetica.drawString( "Are you sure you want to restart? Y / N ", ofGetWidth() / 2 - iScaler * 13, ofGetHeight() / 2.0 );
}

//--------------------------------------------------------------
void testApp::fDrawStaff() {
    
    // Draw the staff with transparency.
    if ( gameState < 3 ) {
        ofSetColor( 0, int( iStaffAlpha ) );
    } else {
        ofSetColor(0);
    }
    
    // Establish some reference floats.
    float staffStart = ( ofGetHeight() - iScaler * 8 ) * staffBracket.getWidth() / staffBracket.getHeight();
    float xStart;
    if ( myPlayer.pos.x < staffStart + ofGetWidth() ) {
        xStart = staffStart;
    } else {
        xStart = myPlayer.pos.x - ofGetWidth();
    }
    
    ofSetLineWidth( 1 );
    
    // Draw the horizontal staff lines.
    for ( int i = 2; i < 7; i++ ) {
        ofLine( xStart, iScaler * 2 * i, myPlayer.pos.x + ofGetWidth(), iScaler * 2 * i );
        ofLine( xStart, ofGetHeight() - ( iScaler * 2 * i ), myPlayer.pos.x + ofGetWidth(), ofGetHeight() - ( iScaler * 2 * i ) );
    }
    
    if ( gameState >= 3 ) {
        
        // Draw initial vertical line.
        ofSetLineWidth( 3 );
        ofLine( xStart, iScaler * 4, xStart, ofGetHeight() - iScaler * 4 );
        ofSetLineWidth( 1 );
        
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
        
        helvetica.drawString( "Finished! Press SHIFT + R to quick-restart.", iScaler * 800, ofGetHeight() / 2 );
        
        for ( int i = 0; i < 30; i++ ) {
            // Draw the measure lines.
            ofLine( iScaler * 18 + fMeasureLength * i, iScaler * 4,  iScaler * 18 + fMeasureLength * i, ofGetHeight() - iScaler * 4 );
            // Draw the measure number.
            helvetica.drawString( ofToString( i + 1 ), iScaler * 19 + fMeasureLength * i, iScaler * 4);
        }
    }
}

//--------------------------------------------------------------
void testApp::fLoadPrototype() {
    
    cleanup();
    myPlayer.setup( iScaler, bUsingController, ofVec2f( iScaler * 4, iThirdOfScreen ) );
    
    float fZoomedInX = iScaler * 7.5;
    
    if ( gameState == 1 ) {
        
        bCamZoomedIn = true;
        myPlayer.pos.x = fZoomedInX;
        addObject( myTutorial.setup( iScaler, iThirdOfScreen, bIsLefty ) );
        obstacleList = myTutorial.obstacleList;
        
        objectList[ objectList.size() - 1 ].vel.set( float( -( iScaler / 8.3333 ) ), 0.0 );
        
    } else if ( gameState == 2 ) {
        
        bCamZoomedIn = true;
        myPlayer.pos.x = fZoomedInX;
        addObject( myBoss.setup( iScaler, fMeasureLength ) );
        
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].vel.set( float( -( iScaler / 5.0 ) ), 0.0 );
        }
        
        bHighlightNote = true;
        getThisOne = 0;
        
    } else if ( gameState >= 3 ) {
        
        myPlayer2.setup( iScaler, bUsingController, ofVec2f( iScaler * 4, ofGetHeight() - iThirdOfScreen + iScaler * 5 ) );
        
        addObject( myTrack.setup( iScaler, fMeasureLength ) );
        
        if ( gameState == 4 ) {
            bCamZoomedIn = true;
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

// Again, from the ofxGamepad example:

void testApp::axisChanged(ofxGamepadAxisEvent& e)
{
    float fStickBuffer = 0.125;
    
    if ( gameState == 3 ) {
        // P1 vertical movement
        if ( e.axis == 5 ) {
            if ( abs( e.value ) > fStickBuffer ) {
                myPlayer.vel.y = ofMap( e.value, -1, 1, -myPlayer.maxVel, myPlayer.maxVel );
                //cout << "AXIS " << e.axis << " VALUE " << ofToString(e.value) << endl;
            } else {
                myPlayer.vel.y = 0;
            }
        }
        // P2 vertical movement
        if ( e.axis == 3 ) {
            if ( abs( e.value ) > fStickBuffer ) {
                myPlayer2.vel.y = ofMap( e.value, -1, 1, -myPlayer2.maxVel, myPlayer2.maxVel );
                //cout << "AXIS " << e.axis << " VALUE " << ofToString(e.value) << endl;
            } else {
                myPlayer2.vel.y = 0;
            }
        }
    } else if ( gameState == 4 ) {
        if ( e.axis == 3 ) {
            if ( abs( e.value ) > fStickBuffer ) {
                if ( e.value < 0 ) {
                    myPlayer.applyForce( ofVec2f( 0.0, -2.0 ) );
                }
            }
        }
    }
}

void testApp::buttonPressed(ofxGamepadButtonEvent& e)
{
    cout << "BUTTON " << e.button << " RELEASED" << endl;
}

void testApp::buttonReleased(ofxGamepadButtonEvent& e)
{
    cout << "BUTTON " << e.button << " PRESSED" << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch ( key ) {
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
            } else {
                myPlayer2.up = true;
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
            } else {
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
            } else {
                myPlayer2.down = true;
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
            } else {
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
                myPlayer2.up = false;
            }
            break;
        case OF_KEY_UP:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.up = false;
                    myPlayer.allowJump = true;
                }
            } else {
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
                myPlayer2.down = false;
            }
            break;
        case OF_KEY_DOWN:
            if ( gameState < 3 ) {
                if ( bIsLefty ) {
                    myPlayer.down = false;
                } else {
                    //
                }
            } else {
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
            }
            break;
            
            // End movement and action.
            //----------------------------------------------------
    }
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