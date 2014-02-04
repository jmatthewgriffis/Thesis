#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // Necessary for a standalone app.
    //ofSetDataPathRoot("data/");
    
    // Switch debug mode on and off.
    bIsDebugging = false;
    
    // Support r-restart:
    cleanup();
    lastTime = ofGetElapsedTimef();
    
    frameRate = 60;
    objectLife = 7 * frameRate;
    
    // Maintenance
    ofSetVerticalSync( true );
    ofEnableAlphaBlending();
    ofSetFrameRate( frameRate );
    ofSetCircleResolution( 100 );
    myCam.disableMouseInput();
    CGDisplayHideCursor(NULL); // ofHideCursor() does not work, but this does.
    
    helvetica.loadFont( "fonts/helvetica.otf", 24 );
    
    // Background
    ofBackground( 255 );
    
    // Run a test pattern and highlight the first element.
    //    testPattern();
    bHighlightNote = false;
    if ( bHighlightNote ) getThisOne = 0;
    else getThisOne = -1;
    
    iStaffAlphaMin = 10;
    iStaffAlphaMax = 200;
    iStaffAlpha = iStaffAlphaMin;
    iStaffAlphaVel = 0.5;
    
    bIsLefty = bIsRecording = false;
    
    //myPlayer.setup();
    //myPlayer.setup( ofVec2f( 10200, 100 ) );
    myPlayer.setup( ofVec2f( 100, 50 ) );
    myPlayer2.setup( ofVec2f( 100, ofGetHeight() - 50 ) );
    
    // 0 is title screen, -1 is restart screen, 1 is game screen, 2 is boss screen.
    gameState = 1;
    currentState = 1;
    // To jump straight to the boss battle, must set gameState to 2, then uncomment the following...
    
    /*testPattern();
     bHighlightNote = true;
     getThisOne = 0;*/
    
    // ...and comment out the following:
    
    fSetupTutorial();
}

//--------------------------------------------------------------
bool bShouldIErase( Object &a ){
    
    // Zach Lieberman showed me this method to remove an element from a vector. We create a boolean function, feed it a class, and pass a reference label that we make up (in this case 'a') so we can refer to the applicable object. Then we check for a certain condition which if met returns a boolean value of 'true.' Otherwise it returns 'false.'
    
    if ( a.destroyMe ) return true;
    else return false;
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Update the control text based on the player's choice on the title screen.
    if ( gameState == 0 ) fWriteControls();
    
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
    //myPlayer.applyForce( ofVec2f( 0.0, 0.3 ) );
    
    // Run collision detection.
    //playerCollidesWithGround();
    playerCollidesWithObstacle();
    playerCollidesWithObject();
    objectCollidesWithObstacle();
    
    // Update the player (duh).
    myPlayer.update();
    myPlayer2.update();
    
    // Update the notes.
    updateObjectList();
    
    // Following up the boolean function we created above, this oF function sorts the vector according to the values of the booleans and then removes any with a 'true' value:
    ofRemove( objectList, bShouldIErase );
    ofRemove( recordedList, bShouldIErase );
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if ( gameState == 0 ) {
        fDrawTitleScreen();
    }
    
    else if ( gameState == -1 ) {
        fDrawRestartScreen();
    }
    
    // Don't draw anything else if not on the game screen.
    else if ( gameState > 0 ) {
        
        if ( gameState == 1 ) {
            myCam.begin();
            myCam.setupPerspective();
        }
        
        // LOCATION-INDEPENDENT
        
        // Move the camera with the player, as long as it dosn't move out of bounds.
        if ( gameState == 1 ) {
            if ( myPlayer.pos.x - ofGetWidth() / 2.0 >= 0 ) {
                myCam.move( myPlayer.pos.x - ofGetWidth() / 2.0, 0, 0 );
            }
        }
        
        // Draw the staff with transparency.
        ofSetColor( 0, int( iStaffAlpha ) );
        float numSections = float(numLines + 1);
        for ( int i = 2; i < ( numSections - 1 ); i++ ) {
            if ( i != 7 ) {
                ofLine( myPlayer.pos.x - ofGetWidth(), ( ofGetHeight() / numSections ) * i, myPlayer.pos.x + ofGetWidth(), ( ofGetHeight() / numSections ) * i );
            }
        }
        
        // Draw some lines on the ground to give something to move against.
        /*ofSetColor( 0 );
         for ( int i = 0; i < 3000; i += 50 ) {
         ofLine( 50 + ( i * 2 ), ofGetHeight(), 50 + i, ofGetHeight() - 25 );
         }*/
        
        // LOCATION-DEPENDENT
        
        if ( gameState == 1 ) {
            //fDrawTutorialInstructions();
        } else if ( gameState == 2 ) {
            testPattern();
            helvetica.drawString( "This is not yet fully implemented. :(\nCheck back later.", 100, 100 );
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
        myPlayer2.draw( helvetica, recordedList );
        
        if ( gameState == 1 ) {
            myCam.end();
        }
    }
}

//--------------------------------------------------------------
void testApp::addObject( string _note, float _xPos, int _age ) {
    
    // This function adds an NPC Object.
    Object tmp;
    tmp.setup( _note, _age );
    tmp.pos.x = _xPos;
    objectList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::addRecordedObject( string _note, ofVec2f _vel, int _age ) {
    
    // This function copies a recorded Object into a static vector that gets neither updated nor drawn.
    Object tmp;
    tmp.setup( _note, _age );
    tmp.vel.set( _vel );
    recordedList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::addReplayedObject( string _note, ofVec2f _vel, int _age ) {
    
    // This function copies an Object from the "recorded" vector to the main Object vector. It also reverses horizontal velocity if needed so the Object can travel the other direction.
    Object tmp;
    tmp.setup( _note, _age );
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
            if ( dist.lengthSquared() < ( 50 * 50 ) ) {
                
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
            objectList[ i ].vol = ofMap( myPlayer.fHealth, 0, myPlayer.fHealthMax, 0.0, 1.0 );
        } else {
            objectList[ i ].vol = ofMap( myPlayer2.fHealth, 0, myPlayer2.fHealthMax, 0.0, 1.0 );
        }
        
        objectList[ i ].update( myPlayer.pos );
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
     } else { // FIND ME--I may need to adjust the below in case notes aren't captured linearly.
     xDist = objectList[ i ].pos.x - objectList[ i - 1 ].pos.x;
     }*/
    addRecordedObject( objectList[ _i ].whichNote, objectList[ _i ].vel, objectLife );
    
    // This prevents additional recording calls before the action completes.
    bIsRecording = true;
    
    if ( bHighlightNote && _i != getThisOne ) {
        //        cout<<"yes!"<<endl;
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
    
    if ( myPlayer.pos.y >= ofGetHeight() - myPlayer.tall / 2.0 ) {
        myPlayer.pos.y = ofGetHeight() - myPlayer.tall / 2.0;
        myPlayer.onSurface = true;
    }
}

//--------------------------------------------------------------
void testApp::playerCollidesWithObject() {
    
    // Collision with main objects vector.
    for ( int i = 0; i < objectList.size(); i++ ) {
        
        // Make some floats for shorthand...
        // Player is drawn from the center.
        // Obstacles are drawn from the corner.
        float margin = 10.0;
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
        
        float fHealthMultiplier = 1.5;
        if ( playerRight > objectLeft
            && playerLeft < objectRight
            && playerBottom > objectTop
            && playerTop < objectBottom ) {
            myPlayer.fHealth += myPlayer.fHealthLossSpeed * fHealthMultiplier;
        }
        if ( player2Right > objectLeft
            && player2Left < objectRight
            && player2Bottom > objectTop
            && player2Top < objectBottom ) {
            myPlayer2.fHealth += myPlayer2.fHealthLossSpeed * fHealthMultiplier;
        }
        
        /*
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
         */
        
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
        float margin = 10.0;
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
void testApp::testPattern() {
    
    if ( objectList.size() > 0 ) {
        return;
    }
    
    addObject( "a3", 200, -1 );
    addObject( "c4", 400, -1 );
    addObject( "e4", 600, -1 );
    addObject( "a3", 800, -1 );
    addObject( "a3", 1000, -1 );
    addObject( "d4", 1000, -1 );
    
    for ( int i = 0; i < objectList.size(); i++ ) {
        objectList[ i ].vel.set( -5.0, 0.0 );
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
        //        objectList[ i ].myNote.sound.stop();
        for ( int j = 0; j < objectList[ i ].noteList.size(); j++ ) {
            objectList[ i ].noteList[ j ].sound.stop();
        }
    }
    for ( int i = 0; i < recordedList.size(); i++ ) {
        //        recordedList[ i ].myNote.sound.stop();
        for ( int j = 0; j < recordedList[ i ].noteList.size(); j++ ) {
            recordedList[ i ].noteList[ j ].sound.stop();
        }
    }
    
    // Clear the vector.
    obstacleList.clear();
    objectList.clear();
    recordedList.clear();
}

//--------------------------------------------------------------
void testApp::fDrawTitleScreen() {
    
    ofSetRectMode( OF_RECTMODE_CORNER );
    
    ofSetColor( 0 );
    helvetica.drawString( "Hello! Welcome to Protoville, home of the prototype.", 125, 75 );
    helvetica.drawString( "Please choose your control affiliation.", 225, 125 );
    
    if ( bIsLefty) ofSetColor( 0 );
    else {
        ofSetColor( 0 );
        ofRect( 190, 170, 670, 50 );
        ofSetColor( 255 );
    }
    helvetica.drawString( "I am so dextrous (right-handed).", 275, 200 );
    if ( bIsLefty) {
        ofSetColor( 0 );
        ofRect( 190, 220, 670, 50 );
        ofSetColor( 255 );
    } else ofSetColor( 0 );
    helvetica.drawString( "I feel quite sinister (left-handed).", 275, 250 );
    
    ofSetColor( 0 );
    
    ofPushMatrix();{
        ofTranslate( ofGetWidth() - 75, 225 );
        helvetica.drawString( "[^]", 0, 0 );
        ofRotate( 180 );
        helvetica.drawString( "[^]", 15, 17 );
    }ofPopMatrix();
    
    helvetica.drawString( "Headphones recommended.", 300, 310 );
    helvetica.drawString( "Press ENTER to continue.", 300, 360 );
}

//--------------------------------------------------------------
void testApp::fDrawRestartScreen() {
    
    ofSetColor( 0 );
    helvetica.drawString( "Are you sure you want to restart? Y / N ", 200, ofGetHeight() / 2.0 );
}

//--------------------------------------------------------------
void testApp::fDrawTutorialInstructions() {
    
    float y = 100;
    // Left and right
    helvetica.drawString( sLeft + " / " + sRight, 100, y );
    // Up
    helvetica.drawString( sUp, 1350, y );
    // Record
    helvetica.drawString( sAltLeft, 3000, y );
    // Replay
    helvetica.drawString( sAltRight, 3800, y );
    
    // Sly note
    helvetica.drawString( "Warning: the rest is under construction.\nPlease wear a hard hat.\nBut feel free to wander into the infinite reaches.\n(Press [R] to restart.)", 12100, y );
    
    // Cue boss battle.
    helvetica.drawString( "[ENTER] here for a boss battle!", 10500, y + 125 );
}

void testApp::fSetupTutorial() {
    
    //m1
    
    //riff bass
    float m1Bass = 500;
    float spacer = 150;
    float mLength = spacer * 5.333333;
    //float mLength = 800;
    //cout<<mLength<<endl;
    
    addObject("c3_middle", m1Bass-300, -1); // test note
    
    addObject("d2#", m1Bass, -1);
    addObject("a2#", m1Bass, -1);
    addObject("d2#", m1Bass + spacer, -1);
    addObject("a2#", m1Bass + spacer, -1);
    addObject("d2#", m1Bass + spacer * 2, -1);
    addObject("a2#", m1Bass + spacer * 2, -1);
    addObject("d2#", m1Bass + spacer * 3, -1);
    addObject("a2#", m1Bass + spacer * 3, -1);
    addObject("d2#", m1Bass + spacer * 4, -1);
    addObject("a2#", m1Bass + spacer * 4, -1);
    
    
    //riff treble
    float m1Treble = m1Bass + 100;
    addObject("f3#", m1Treble, -1);
    addObject("d3#", m1Treble + 100, -1);
    addObject("c3#", m1Treble + 300, -1);
    addObject("d3#", m1Treble + 400, -1);
    addObject("f3#", m1Treble + 610, -1);
    
    //m2
    
    //riff bass
    float m2Bass = m1Bass + mLength;
    
    addObject("d2#", m2Bass, -1);
    addObject("a2#", m2Bass, -1);
    addObject("d2#", m2Bass + spacer, -1);
    addObject("a2#", m2Bass + spacer, -1);
    addObject("d2#", m2Bass + spacer * 2, -1);
    addObject("a2#", m2Bass + spacer * 2, -1);
    addObject("d2#", m2Bass + spacer * 3, -1);
    addObject("a2#", m2Bass + spacer * 3, -1);
    addObject("d2#", m2Bass + spacer * 4, -1);
    addObject("a2#", m2Bass + spacer * 4, -1);
    
    
    //riff treble
    float m2Treble = m2Bass + 100;
    addObject("f3#", m2Treble, -1);
    addObject("d3#", m2Treble + 100, -1);
    addObject("c3#", m2Treble + 300, -1);
    addObject("d3#", m2Treble + 400, -1);
    addObject("f3#", m2Treble + 600, -1);
    
    //m3
    
    //riff bass
    float m3Bass = m2Bass + mLength;
    
    addObject("d2#", m3Bass, -1);
    addObject("a2#", m3Bass, -1);
    addObject("d2#", m3Bass + spacer, -1);
    addObject("a2#", m3Bass + spacer, -1);
    addObject("d2#", m3Bass + spacer * 2, -1);
    addObject("a2#", m3Bass + spacer * 2, -1);
    addObject("d2#", m3Bass + spacer * 3, -1);
    addObject("a2#", m3Bass + spacer * 3, -1);
    addObject("d2#", m3Bass + spacer * 4, -1);
    addObject("a2#", m3Bass + spacer * 4, -1);
    
    
    //riff treble
    float m3Treble = m3Bass + 100;
    addObject("f3#", m3Treble, -1);
    addObject("d3#", m3Treble + 100, -1);
    addObject("c3#", m3Treble + 300, -1);
    addObject("d3#", m3Treble + 400, -1);
    addObject("f3#", m3Treble + 600, -1);
    
    //m4
    
    //riff bass
    float m4Bass = m3Bass + mLength;
    
    addObject("d2#", m4Bass, -1);
    addObject("a2#", m4Bass, -1);
    addObject("d2#", m4Bass + spacer, -1);
    addObject("a2#", m4Bass + spacer, -1);
    addObject("d2#", m4Bass + spacer * 2, -1);
    addObject("a2#", m4Bass + spacer * 2, -1);
    addObject("d2#", m4Bass + spacer * 3, -1);
    addObject("a2#", m4Bass + spacer * 3, -1);
    addObject("d2#", m4Bass + spacer * 4, -1);
    addObject("a2#", m4Bass + spacer * 4, -1);
    
    
    //riff treble
    float m4Treble = m4Bass + 100;
    addObject("f3#", m4Treble, -1);
    addObject("d3#", m4Treble + 100, -1);
    addObject("c3#", m4Treble + 300, -1);
    addObject("d3#", m4Treble + 400, -1);
    addObject("f3#", m4Treble + 600, -1);
    
    //change
    
    //m5
    
    //riff bass
    float m5Bass = m4Bass + mLength;
    
    addObject("c2#", m5Bass, -1);
    addObject("g2#", m5Bass, -1);
    addObject("c2#", m5Bass + spacer, -1);
    addObject("g2#", m5Bass + spacer, -1);
    addObject("c2#", m5Bass + spacer * 2, -1);
    addObject("g2#", m5Bass + spacer * 2, -1);
    addObject("c2#", m5Bass + spacer * 3, -1);
    addObject("g2#", m5Bass + spacer * 3, -1);
    addObject("c2#", m5Bass + spacer * 4, -1);
    addObject("g2#", m5Bass + spacer * 4, -1);
    
    
    //riff treble
    float m5Treble = m5Bass + 100;
    addObject("f3#", m5Treble, -1);
    addObject("d3#", m5Treble + 100, -1);
    addObject("c3#", m5Treble + 300, -1);
    addObject("d3#", m5Treble + 400, -1);
    addObject("f3#", m5Treble + 600, -1);
    
    //m6
    
    //riff bass
    float m6Bass = m5Bass + mLength;
    
    addObject("c2#", m6Bass, -1);
    addObject("g2#", m6Bass, -1);
    addObject("c2#", m6Bass + spacer, -1);
    addObject("g2#", m6Bass + spacer, -1);
    addObject("c2#", m6Bass + spacer * 2, -1);
    addObject("g2#", m6Bass + spacer * 2, -1);
    addObject("c2#", m6Bass + spacer * 3, -1);
    addObject("g2#", m6Bass + spacer * 3, -1);
    addObject("c2#", m6Bass + spacer * 4, -1);
    addObject("g2#", m6Bass + spacer * 4, -1);
    
    
    //riff treble
    float m6Treble = m6Bass + 100;
    addObject("f3#", m6Treble, -1);
    addObject("d3#", m6Treble + 100, -1);
    addObject("c3#", m6Treble + 300, -1);
    addObject("d3#", m6Treble + 400, -1);
    addObject("f3#", m6Treble + 600, -1);
    
    //m7
    
    //riff bass
    float m7Bass = m6Bass + mLength;
    
    addObject("c2#", m7Bass, -1);
    addObject("g2#", m7Bass, -1);
    addObject("c2#", m7Bass + spacer, -1);
    addObject("g2#", m7Bass + spacer, -1);
    addObject("c2#", m7Bass + spacer * 2, -1);
    addObject("g2#", m7Bass + spacer * 2, -1);
    addObject("c2#", m7Bass + spacer * 3, -1);
    addObject("g2#", m7Bass + spacer * 3, -1);
    addObject("c2#", m7Bass + spacer * 4, -1);
    addObject("g2#", m7Bass + spacer * 4, -1);
    
    
    //riff treble
    float m7Treble = m7Bass + 100;
    addObject("f3#", m7Treble, -1);
    addObject("d3#", m7Treble + 100, -1);
    addObject("c3#", m7Treble + 300, -1);
    addObject("d3#", m7Treble + 400, -1);
    addObject("f3#", m7Treble + 600, -1);
    
    //m8
    
    //riff bass
    float m8Bass = m7Bass + mLength;
    
    addObject("c2#", m8Bass, -1);
    addObject("g2#", m8Bass, -1);
    addObject("c2#", m8Bass + spacer, -1);
    addObject("g2#", m8Bass + spacer, -1);
    addObject("c2#", m8Bass + spacer * 2, -1);
    addObject("g2#", m8Bass + spacer * 2, -1);
    addObject("c2#", m8Bass + spacer * 3, -1);
    addObject("g2#", m8Bass + spacer * 3, -1);
    addObject("c2#", m8Bass + spacer * 4, -1);
    addObject("g2#", m8Bass + spacer * 4, -1);
    
    
    //riff treble
    float m8Treble = m8Bass + 100;
    addObject("f3#", m8Treble, -1);
    addObject("d3#", m8Treble + 100, -1);
    addObject("c3#", m8Treble + 300, -1);
    addObject("d3#", m8Treble + 400, -1);
    addObject("f3#", m8Treble + 600, -1);
    
    // change
    
    //m9
    
    //riff bass
    float m9Bass = m8Bass + mLength;
    
    addObject("d2#", m9Bass, -1);
    addObject("a2#", m9Bass, -1);
    addObject("d2#", m9Bass + spacer, -1);
    addObject("a2#", m9Bass + spacer, -1);
    addObject("d2#", m9Bass + spacer * 2, -1);
    addObject("a2#", m9Bass + spacer * 2, -1);
    addObject("d2#", m9Bass + spacer * 3, -1);
    addObject("a2#", m9Bass + spacer * 3, -1);
    addObject("d2#", m9Bass + spacer * 4, -1);
    addObject("a2#", m9Bass + spacer * 4, -1);
    
    
    //riff treble
    float m9Treble = m9Bass + 100;
    addObject("f3#", m9Treble, -1);
    addObject("d3#", m9Treble + 100, -1);
    addObject("c3#", m9Treble + 300, -1);
    addObject("d3#", m9Treble + 400, -1);
    addObject("f3#", m9Treble + 600, -1);
    
    //m10
    
    //riff bass
    float m10Bass = m9Bass + mLength;
    
    addObject("d2#", m10Bass, -1);
    addObject("a2#", m10Bass, -1);
    addObject("d2#", m10Bass + spacer, -1);
    addObject("a2#", m10Bass + spacer, -1);
    addObject("d2#", m10Bass + spacer * 2, -1);
    addObject("a2#", m10Bass + spacer * 2, -1);
    addObject("d2#", m10Bass + spacer * 3, -1);
    addObject("a2#", m10Bass + spacer * 3, -1);
    addObject("d2#", m10Bass + spacer * 4, -1);
    addObject("a2#", m10Bass + spacer * 4, -1);
    
    
    //riff treble
    float m10Treble = m10Bass + 100;
    addObject("f3#", m10Treble, -1);
    addObject("d3#", m10Treble + 100, -1);
    addObject("c3#", m10Treble + 300, -1);
    addObject("d3#", m10Treble + 400, -1);
    addObject("f3#", m10Treble + 600, -1);
    
    //m11
    
    //riff bass
    float m11Bass = m10Bass + mLength;
    
    addObject("d2#", m11Bass, -1);
    addObject("a2#", m11Bass, -1);
    addObject("d2#", m11Bass + spacer, -1);
    addObject("a2#", m11Bass + spacer, -1);
    addObject("d2#", m11Bass + spacer * 2, -1);
    addObject("a2#", m11Bass + spacer * 2, -1);
    addObject("d2#", m11Bass + spacer * 3, -1);
    addObject("a2#", m11Bass + spacer * 3, -1);
    addObject("d2#", m11Bass + spacer * 4, -1);
    addObject("a2#", m11Bass + spacer * 4, -1);
    
    
    //riff treble
    float m11Treble = m11Bass + 100;
    addObject("f3#", m11Treble, -1);
    addObject("d3#", m11Treble + 100, -1);
    addObject("c3#", m11Treble + 300, -1);
    addObject("d3#", m11Treble + 400, -1);
    addObject("f3#", m11Treble + 600, -1);
    
    //m12
    
    //riff bass
    float m12Bass = m11Bass + mLength;
    
    addObject("d2#", m12Bass, -1);
    addObject("a2#", m12Bass, -1);
    addObject("d2#", m12Bass + spacer, -1);
    addObject("a2#", m12Bass + spacer, -1);
    addObject("d2#", m12Bass + spacer * 2, -1);
    addObject("a2#", m12Bass + spacer * 2, -1);
    addObject("d2#", m12Bass + spacer * 3, -1);
    addObject("a2#", m12Bass + spacer * 3, -1);
    addObject("d2#", m12Bass + spacer * 4, -1);
    addObject("a2#", m12Bass + spacer * 4, -1);
    
    
    //riff treble
    float m12Treble = m12Bass + 100;
    addObject("f3#", m12Treble, -1);
    addObject("d3#", m12Treble + 100, -1);
    addObject("c3#", m12Treble + 300, -1);
    addObject("d3#", m12Treble + 400, -1);
    addObject("f3#", m12Treble + 600, -1);
    
    //change
    
    //m13
    
    //riff bass
    float m13Bass = m12Bass + mLength;
    
    addObject("c2#", m13Bass, -1);
    addObject("g2#", m13Bass, -1);
    addObject("c2#", m13Bass + spacer, -1);
    addObject("g2#", m13Bass + spacer, -1);
    addObject("c2#", m13Bass + spacer * 2, -1);
    addObject("g2#", m13Bass + spacer * 2, -1);
    addObject("c2#", m13Bass + spacer * 3, -1);
    addObject("g2#", m13Bass + spacer * 3, -1);
    addObject("c2#", m13Bass + spacer * 4, -1);
    addObject("g2#", m13Bass + spacer * 4, -1);
    
    
    //riff treble
    float m13Treble = m13Bass + 100;
    addObject("f3#", m13Treble, -1);
    addObject("d3#", m13Treble + 100, -1);
    addObject("c3#", m13Treble + 300, -1);
    addObject("d3#", m13Treble + 400, -1);
    addObject("f3#", m13Treble + 600, -1);
    
    //m14
    
    //riff bass
    float m14Bass = m13Bass + mLength;
    
    addObject("c2#", m14Bass, -1);
    addObject("g2#", m14Bass, -1);
    addObject("c2#", m14Bass + spacer, -1);
    addObject("g2#", m14Bass + spacer, -1);
    addObject("c2#", m14Bass + spacer * 2, -1);
    addObject("g2#", m14Bass + spacer * 2, -1);
    addObject("c2#", m14Bass + spacer * 3, -1);
    addObject("g2#", m14Bass + spacer * 3, -1);
    addObject("c2#", m14Bass + spacer * 4, -1);
    addObject("g2#", m14Bass + spacer * 4, -1);
    
    
    //riff treble
    float m14Treble = m14Bass + 100;
    addObject("f3#", m14Treble, -1);
    addObject("d3#", m14Treble + 100, -1);
    addObject("c3#", m14Treble + 300, -1);
    addObject("d3#", m14Treble + 400, -1);
    addObject("f3#", m14Treble + 600, -1);
    
    //m15
    
    //riff bass
    float m15Bass = m14Bass + mLength;
    
    addObject("c2#", m15Bass, -1);
    addObject("g2#", m15Bass, -1);
    addObject("c2#", m15Bass + spacer, -1);
    addObject("g2#", m15Bass + spacer, -1);
    addObject("c2#", m15Bass + spacer * 2, -1);
    addObject("g2#", m15Bass + spacer * 2, -1);
    addObject("c2#", m15Bass + spacer * 3, -1);
    addObject("g2#", m15Bass + spacer * 3, -1);
    addObject("c2#", m15Bass + spacer * 4, -1);
    addObject("g2#", m15Bass + spacer * 4, -1);
    
    
    //riff treble
    float m15Treble = m15Bass + 100;
    addObject("f3#", m15Treble, -1);
    addObject("d3#", m15Treble + 100, -1);
    addObject("c3#", m15Treble + 300, -1);
    addObject("d3#", m15Treble + 400, -1);
    addObject("f3#", m15Treble + 600, -1);
    
    //m16
    
    //riff bass
    float m16Bass = m15Bass + mLength;
    
    addObject("c2#", m16Bass, -1);
    addObject("g2#", m16Bass, -1);
    addObject("c2#", m16Bass + spacer, -1);
    addObject("g2#", m16Bass + spacer, -1);
    addObject("c2#", m16Bass + spacer * 2, -1);
    addObject("g2#", m16Bass + spacer * 2, -1);
    addObject("c2#", m16Bass + spacer * 3, -1);
    addObject("g2#", m16Bass + spacer * 3, -1);
    addObject("c2#", m16Bass + spacer * 4, -1);
    addObject("g2#", m16Bass + spacer * 4, -1);
    
    
    //riff treble
    float m16Treble = m16Bass + 100;
    addObject("f3#", m16Treble, -1);
    addObject("d3#", m16Treble + 100, -1);
    addObject("c3#", m16Treble + 300, -1);
    addObject("d3#", m16Treble + 400, -1);
    addObject("f3#", m16Treble + 600, -1);
    
    //change
    
    //m17
    
    //riff bass
    float m17Bass = m16Bass + mLength;
    
    addObject("b1", m17Bass, -1);
    addObject("f2#", m17Bass, -1);
    addObject("b1", m17Bass + spacer, -1);
    addObject("f2#", m17Bass + spacer, -1);
    addObject("b1", m17Bass + spacer * 2, -1);
    addObject("f2#", m17Bass + spacer * 2, -1);
    addObject("b1", m17Bass + spacer * 3, -1);
    addObject("f2#", m17Bass + spacer * 3, -1);
    addObject("b1", m17Bass + spacer * 4, -1);
    addObject("f2#", m17Bass + spacer * 4, -1);
    
    
    //riff treble
    float m17Treble = m17Bass + 100;
    addObject("f3#", m17Treble, -1);
    addObject("d3#", m17Treble + 100, -1);
    addObject("c3#", m17Treble + 300, -1);
    addObject("d3#", m17Treble + 400, -1);
    addObject("f3#", m17Treble + 600, -1);
    
    //change
    
    //m18
    
    //riff bass
    float m18Bass = m17Bass + mLength;
    
    addObject("c2#", m18Bass, -1);
    addObject("g2#", m18Bass, -1);
    addObject("c2#", m18Bass + spacer, -1);
    addObject("g2#", m18Bass + spacer, -1);
    addObject("c2#", m18Bass + spacer * 2, -1);
    addObject("g2#", m18Bass + spacer * 2, -1);
    addObject("c2#", m18Bass + spacer * 3, -1);
    addObject("g2#", m18Bass + spacer * 3, -1);
    addObject("c2#", m18Bass + spacer * 4, -1);
    addObject("g2#", m18Bass + spacer * 4, -1);
    
    
    //riff treble
    float m18Treble = m18Bass + 100;
    addObject("f3#", m18Treble, -1);
    addObject("d3#", m18Treble + 100, -1);
    addObject("c3#", m18Treble + 300, -1);
    addObject("d3#", m18Treble + 400, -1);
    addObject("f3#", m18Treble + 600, -1);
    
    //change
    
    //m19
    
    //riff bass
    float m19Bass = m18Bass + mLength;
    
    addObject("d2#", m19Bass, -1);
    addObject("a2#", m19Bass, -1);
    addObject("d2#", m19Bass + spacer, -1);
    addObject("a2#", m19Bass + spacer, -1);
    addObject("d2#", m19Bass + spacer * 2, -1);
    addObject("a2#", m19Bass + spacer * 2, -1);
    addObject("d2#", m19Bass + spacer * 3, -1);
    addObject("a2#", m19Bass + spacer * 3, -1);
    addObject("d2#", m19Bass + spacer * 4, -1);
    addObject("a2#", m19Bass + spacer * 4, -1);
    
    
    //riff treble
    float m19Treble = m19Bass + 100;
    addObject("f3#", m19Treble, -1);
    addObject("d3#", m19Treble + 100, -1);
    addObject("c3#", m19Treble + 300, -1);
    addObject("d3#", m19Treble + 400, -1);
    addObject("f3#", m19Treble + 600, -1);
    
    //m20
    
    //riff bass
    float m20Bass = m19Bass + mLength;
    
    addObject("d2#", m20Bass, -1);
    addObject("a2#", m20Bass, -1);
    addObject("d2#", m20Bass + spacer, -1);
    addObject("a2#", m20Bass + spacer, -1);
    addObject("d2#", m20Bass + spacer * 2, -1);
    addObject("a2#", m20Bass + spacer * 2, -1);
    addObject("d2#", m20Bass + spacer * 3, -1);
    addObject("a2#", m20Bass + spacer * 3, -1);
    addObject("d2#", m20Bass + spacer * 4, -1);
    addObject("a2#", m20Bass + spacer * 4, -1);
    
    
    //riff treble
    float m20Treble = m20Bass + 100;
    addObject("f3#", m20Treble, -1);
    addObject("d3#", m20Treble + 100, -1);
    addObject("c3#", m20Treble + 300, -1);
    addObject("d3#", m20Treble + 400, -1);
    addObject("f3#", m20Treble + 600, -1);
    
    //change
    
    //m21
    
    //riff bass
    float m21Bass = m20Bass + mLength;
    
    addObject("b1", m21Bass, -1);
    addObject("f2#", m21Bass, -1);
    addObject("b1", m21Bass + spacer, -1);
    addObject("f2#", m21Bass + spacer, -1);
    addObject("b1", m21Bass + spacer * 2, -1);
    addObject("f2#", m21Bass + spacer * 2, -1);
    addObject("b1", m21Bass + spacer * 3, -1);
    addObject("f2#", m21Bass + spacer * 3, -1);
    addObject("b1", m21Bass + spacer * 4, -1);
    addObject("f2#", m21Bass + spacer * 4, -1);
    
    
    //riff treble
    float m21Treble = m21Bass + 100;
    addObject("f3#", m21Treble, -1);
    addObject("d3#", m21Treble + 100, -1);
    addObject("c3#", m21Treble + 300, -1);
    addObject("d3#", m21Treble + 400, -1);
    addObject("f3#", m21Treble + 600, -1);
    
    //change
    
    //m22
    
    //riff bass
    float m22Bass = m21Bass + mLength;
    
    addObject("c2#", m22Bass, -1);
    addObject("g2#", m22Bass, -1);
    addObject("c2#", m22Bass + spacer, -1);
    addObject("g2#", m22Bass + spacer, -1);
    addObject("c2#", m22Bass + spacer * 2, -1);
    addObject("g2#", m22Bass + spacer * 2, -1);
    addObject("c2#", m22Bass + spacer * 3, -1);
    addObject("g2#", m22Bass + spacer * 3, -1);
    addObject("c2#", m22Bass + spacer * 4, -1);
    addObject("g2#", m22Bass + spacer * 4, -1);
    
    
    //riff treble
    float m22Treble = m22Bass + 100;
    addObject("f3#", m22Treble, -1);
    addObject("d3#", m22Treble + 100, -1);
    addObject("c3#", m22Treble + 300, -1);
    addObject("d3#", m22Treble + 400, -1);
    addObject("f3#", m22Treble + 600, -1);
    
    //change
    
    //m23
    
    //riff bass
    float m23Bass = m22Bass + mLength;
    
    addObject("d2#", m23Bass, -1);
    addObject("a2#", m23Bass, -1);
    addObject("d2#", m23Bass + spacer, -1);
    addObject("a2#", m23Bass + spacer, -1);
    addObject("d2#", m23Bass + spacer * 2, -1);
    addObject("a2#", m23Bass + spacer * 2, -1);
    addObject("d2#", m23Bass + spacer * 3, -1);
    addObject("a2#", m23Bass + spacer * 3, -1);
    addObject("d2#", m23Bass + spacer * 4, -1);
    addObject("a2#", m23Bass + spacer * 4, -1);
    
    
    //riff treble
    float m23Treble = m23Bass + 100;
    addObject("f3#", m23Treble, -1);
    addObject("d3#", m23Treble + 100, -1);
    addObject("c3#", m23Treble + 300, -1);
    addObject("d3#", m23Treble + 400, -1);
    addObject("f3#", m23Treble + 600, -1);
    
    //m24
    
    //riff bass
    float m24Bass = m23Bass + mLength;
    
    addObject("d2#", m24Bass, -1);
    addObject("a2#", m24Bass, -1);
    /*addObject("d2#", m24Bass + spacer, -1);
     addObject("a2#", m24Bass + spacer, -1);
     addObject("d2#", m24Bass + spacer * 2, -1);
     addObject("a2#", m24Bass + spacer * 2, -1);
     addObject("d2#", m24Bass + spacer * 3, -1);
     addObject("a2#", m24Bass + spacer * 3, -1);
     addObject("d2#", m24Bass + spacer * 4, -1);
     addObject("a2#", m24Bass + spacer * 4, -1);*/
    
    
    //riff treble
    float m24Treble = m24Bass;
    //    float m24Treble = m24Bass + 100;
    addObject("d3#", m24Treble, -1);
    /*addObject("f3#", m24Treble, -1);
     addObject("d3#", m24Treble + 100, -1);
     addObject("c3#", m24Treble + 300, -1);
     addObject("d3#", m24Treble + 400, -1);
     addObject("f3#", m24Treble + 600, -1);*/
    
    return;
    
    // Jump!
    float x = 1000;
    Obstacle tmp;
    tmp.setup( ofVec2f( x, ofGetHeight() ), 100, 75, true );
    obstacleList.push_back( tmp );
    
    // Jump off a note to overcome an obstacle.
    float x2 = 1800;
    addObject( "d3", x2, -1 );
    Obstacle tmp2;
    tmp2.setup( ofVec2f( x2 + 200, ofGetHeight() ), 100, 150, true );
    obstacleList.push_back( tmp2 );
    
    // Record and replay a note to overcome an obstacle.
    float x3 = 2300;
    addObject( "d3", x3, -1 );
    addObject( "a3", x3 + 745, -1 );
    Obstacle tmp3;
    tmp3.setup( ofVec2f( x3 + 2000, ofGetHeight() ), 100, 150, true );
    obstacleList.push_back( tmp3 );
    
    // Spring off notes to get to higher notes and overcome a tall obstacle.
    float x4 = 4600;
    addObject( "d3", x4, -1 );
    //    addObject( "g4", x4 + 200, -1 );
    addObject( "c4", x4 + 400, -1 );
    addObject( "g3", x4 + 700, -1 );
    
    float x5 = x4 + 1100;
    addObject( "g3", x5, -1 );
    addObject( "d3", x5 + 100, -1 );
    //    addObject( "c5", x5 + 200, -1 );
    Obstacle tmp4;
    tmp4.setup( ofVec2f( x5 + 400, ofGetHeight() ), 100, 300, true );
    obstacleList.push_back( tmp4 );
    
    // Record multiple notes before replaying to overcome a tall obstacle before the notes expire.
    addObject( "b3", x5 + 600, -1 );
    addObject( "e3", x5 + 700, -1 );
    
    float x6 = 7500;
    addObject( "b3", x6, -1 );
    addObject( "e3", x6 + 200, -1 );
    Obstacle tmp5;
    tmp5.setup( ofVec2f( x6 + 1450, ofGetHeight() ), 100, 300, true );
    obstacleList.push_back( tmp5 );
    
    // Ride a moving note to overcome a tall obstacle.
    Obstacle tmp6;
    tmp6.setup( ofVec2f( x6 + 1550, ofGetHeight() ), 100, 200, true );
    obstacleList.push_back( tmp6 );
    Obstacle tmp7;
    tmp7.setup( ofVec2f( x6 + 1650, ofGetHeight() ), 100, 100, true );
    obstacleList.push_back( tmp7 );
    
    float x7 = 9250;
    addObject( "b3", x7, -1 );
    objectList[ objectList.size() - 1 ].vel.set( -3.0, 0.0 );
    Obstacle tmp8;
    tmp8.setup( ofVec2f( x7 + 750, ofGetHeight() ), 100, 300, true );
    obstacleList.push_back( tmp8 );
}

//--------------------------------------------------------------
void testApp::fWriteControls() {
    
    // Here we store the controls as strings we can display on-screen.
    
    if ( !bIsLefty ) {
        
        // Movement
        
        sUp = "[W]";
        sLeft = "[A]";
        sDown = "[S]";
        sRight = "[D]";
        
        // Action
        
        sAltUp = "[UP]";
        sAltLeft = "[LEFT]";
        sAltDown = "[DOWN]";
        sAltRight = "[RIGHT]";
    }
    else {
        
        // Action
        
        sAltUp = "[W]";
        sAltLeft = "[A]";
        sAltDown = "[S]";
        sAltRight = "[D]";
        
        // Movement
        
        sUp = "[UP]";
        sLeft = "[LEFT]";
        sDown = "[DOWN]";
        sRight = "[RIGHT]";
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch ( key ) {
            // Reset
        case 'r':
        case 'R':
            if ( bShiftIsPressed ) {
                setup();
                gameState = 1;
            }
            else {
                if ( gameState > 0 ) {
                    currentState = gameState;
                    gameState = -1;
                }
            }
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
                gameState = 1;
            }
            // Go to boss battle if player has reached end of tutorial.
            /*else if ( gameState == 1 && myPlayer.pos.x > obstacleList[ obstacleList.size() - 1 ].pos.x + obstacleList[ obstacleList.size() - 1 ].wide + 400 ) {
                //else if ( gameState == 1 ) {
                cleanup();
                gameState = 2;
                myPlayer.setup();
                bHighlightNote = true;
                getThisOne = 0;
            }*/
            break;
            
        case OF_KEY_SHIFT:
            bShiftIsPressed = true;
            break;
            
            //----------------------------------------------------
            // Movement and action (depends on the control scheme).
            
            // UP
        case 'w':
        case 'W':
            if ( bIsLefty ) {}
            //else myPlayer.up = true;
            else myPlayer2.up = true;
            break;
        case OF_KEY_UP:
            if ( gameState == 0 ) {
                bIsLefty = !bIsLefty;
            } else {
                if ( bIsLefty ) myPlayer.up = true;
                else { myPlayer.up = true; }
            }
            break;
            
            // LEFT
        case 'a':
        case 'A':
            //if ( bIsLefty ) myPlayer.record = true;
            //else myPlayer.left = true;
            break;
        case OF_KEY_LEFT:
            //if ( bIsLefty ) myPlayer.left = true;
            //else myPlayer.record = true;
            break;
            
            // DOWN
        case 's':
        case 'S':
            if ( bIsLefty ) {}
            //else myPlayer.down = true;
            else myPlayer2.down = true;
            break;
        case OF_KEY_DOWN:
            if ( gameState == 0 ) {
                bIsLefty = !bIsLefty;
            } else {
                if ( bIsLefty ) myPlayer.down = true;
                else { myPlayer.down = true; }
            }
            break;
            
            // RIGHT
        case 'd':
        case 'D':
            if ( bIsLefty ) {
                //myPlayer.replay = true;
                //if ( myPlayer.bAllowReplay ) fReplay();
            }
            //else myPlayer.right = true;
            break;
        case OF_KEY_RIGHT:
            //if ( bIsLefty ) myPlayer.right = true;
            /*else*/ {
                //myPlayer.replay = true;
                //if ( myPlayer.bAllowReplay ) fReplay();
            }
            break;
            
            // End movement and action.
            //----------------------------------------------------
            
            // Debug mode-----------------------------------------
        {
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
                        objectList[ i ].vel.set( -5, 0 );
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
            if ( bIsLefty ) {}
            else {
                //myPlayer.up = false;
                myPlayer2.up = false;
                myPlayer.allowJump = true;
            }
            break;
        case OF_KEY_UP:
            if ( bIsLefty ) {
                myPlayer.up = false;
                myPlayer.allowJump = true;
            }
            else { myPlayer.up = false; }
            break;
            
            // LEFT
        case 'a':
        case 'A':
            //if ( bIsLefty ) myPlayer.record = false;
            //else myPlayer.left = false;
            break;
        case OF_KEY_LEFT:
            //if ( bIsLefty ) myPlayer.left = false;
            //else myPlayer.record = false;
            break;
            
            // DOWN
        case 's':
        case 'S':
            if ( bIsLefty ) {}
            //else myPlayer.down = false;
            else myPlayer2.down = false;
            break;
        case OF_KEY_DOWN:
            if ( bIsLefty ) myPlayer.down = false;
            else { myPlayer.down = false; }
            break;
            
            // RIGHT
        case 'd':
        case 'D':
            //if ( bIsLefty ) myPlayer.replay = false;
            //else myPlayer.right = false;
            break;
        case OF_KEY_RIGHT:
            //if ( bIsLefty ) myPlayer.right = false;
            //else myPlayer.replay = false;
            break;
            
            // End movement and action.
            //----------------------------------------------------
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
