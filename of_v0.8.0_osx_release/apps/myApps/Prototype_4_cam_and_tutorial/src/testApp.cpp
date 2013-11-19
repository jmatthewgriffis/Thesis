#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
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
    
    helvetica.loadFont( "fonts/helvetica.otf", 24 );
    
    // Background
    ofBackground( 255 );
    
    // Make the vector of y-pos.
    staffPosSet();
    
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
    
    myPlayer.setup();
//    myPlayer.setup( ofVec2f( 3000, 100 ) );
    
    // 0 is title screen, 1 is game screen (-1 is restart screen).
    gameState = 0;
    
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
    if ( gameState != 1 ) return;
    
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
    myPlayer.applyForce( ofVec2f( 0.0, 0.3 ) );
    
    // Run collision detection.
    playerCollidesWithGround();
    playerCollidesWithObstacle();
    playerCollidesWithObject();
    
    // Update the player (duh).
    myPlayer.update();
    
    // Update the notes.
    updateObjectList();
    
    // Following up the boolean function we created above, this oF function sorts the vector according to the values of the booleans and then removes any with a 'true' value:
    ofRemove( objectList, bShouldIErase );
    ofRemove( recordedList, bShouldIErase );
}

//--------------------------------------------------------------
void testApp::draw(){
    
    myCam.begin();
    myCam.setupPerspective();
    
    if ( gameState == 0 ) {
        fDrawTitleScreen();
    }
    
    else if ( gameState == -1 ) {
        fDrawRestartScreen();
    }
    
    // Don't draw anything else if not on the game screen.
    else if ( gameState == 1 ) {
        
        // LOCATION-INDEPENDENT
        
        // Move the camera with the player, as long as it dosn't move out of bounds.
        if ( myPlayer.pos.x - ofGetWidth() / 2.0 >= 0 ) {
            myCam.move( myPlayer.pos.x - ofGetWidth() / 2.0, 0, 0 );
        }
        
        // Draw the staff with transparency.
        ofSetColor( 0, int( iStaffAlpha ) );
        for ( int i = 2; i < 7; i++ ) {
            ofLine( myPlayer.pos.x - ofGetWidth(), ( ofGetHeight() / 8.0 ) * i, myPlayer.pos.x + ofGetWidth(), ( ofGetHeight() / 8.0 ) * i );
        }
        
        // Draw some lines on the ground to give something to move against.
        ofSetColor( 0 );
        for ( int i = 0; i < 3000; i += 50 ) {
            ofLine( 50 + ( i * 2 ), ofGetHeight(), 50 + i, ofGetHeight() - 25 );
        }
        
        // LOCATION-DEPENDENT
        
        fDrawTutorialInstructions();
        
        // Draw the notes.
        for ( int i = 0; i < objectList.size(); i++ ) {
            objectList[ i ].draw();
        }
        
        // Draw the obstacles.
        for ( int i = 0; i < obstacleList.size(); i++ ) {
            obstacleList[ i ].draw();
        }
        
        myPlayer.draw();
        
        ofSetColor( 0 );
        if ( myPlayer.bIsReplaying && bIsEmpty ) {
            helvetica.drawString("?", myPlayer.pos.x + 30, myPlayer.pos.y - 30 );
        }
    }
    
    myCam.end();
}

//--------------------------------------------------------------
void testApp::addObject( int _note, float _xPos, int _age ) {
    
    // This function adds an NPC Object.
    Object tmp;
    tmp.setup( _note, staffPosList[ _note ], _age );
    tmp.pos.x = _xPos;
    objectList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::addRecordedObject( int _note, ofVec2f _vel, int _age ) {
    
    // This function copies a recorded Object into a static vector that gets neither updated nor drawn.
    Object tmp;
    tmp.setup( _note, staffPosList[ _note ], _age );
    tmp.vel.set( _vel );
    recordedList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::addReplayedObject( int _note, ofVec2f _vel, int _age ) {
    
    // This function copies an Object from the "recorded" vector to the main Object vector. It also reverses horizontal velocity if needed so the Object can travel the other direction.
    Object tmp;
    tmp.setup( _note, staffPosList[ _note ], _age );
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
        
        objectList[ i ].update( myPlayer.pos );
    }
}

//--------------------------------------------------------------
void testApp::fRecord( int _i ) {
    
    if ( bIsRecording ) return;
    
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
    if ( bIsEmpty ) bIsEmpty = false;
    
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
        bIsEmpty = true;
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
void testApp::testPattern() {
    
    addObject( 2, 200, -1 );
    addObject( 4, 400, -1 );
    addObject( 6, 600, -1 );
    addObject( 2, 800, -1 );
    addObject( 2, 1000, -1 );
    addObject( 5, 1000, -1 );
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
    for ( int i = 0; i < recordedList.size(); i++ ) {
        recordedList[ i ].myNote.sound.stop();
    }
    
    // Clear the vector.
    objectList.clear();
    recordedList.clear();
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
    helvetica.drawString( "Warning: the rest is under construction.\nPlease wear a hard hat.\nBut feel free to wander into the infinite reaches.\n(Press [R] to restart.)", 7000, y );
}

void testApp::fSetupTutorial() {
    
    // Jump!
    float x = 1000;
    Obstacle tmp;
    tmp.setup( ofVec2f( x, ofGetHeight() ), 100, 75, true );
    obstacleList.push_back( tmp );
    
    // Jump off a note to overcome an obstacle.
    float x2 = 1800;
    addObject( 2, x2, -1 );
    Obstacle tmp2;
    tmp2.setup( ofVec2f( x2 + 200, ofGetHeight() ), 100, 150, true );
    obstacleList.push_back( tmp2 );
    
    // Record and replay a note to overcome an obstacle.
    float x3 = 2300;
    addObject( 2, x3, -1 );
    addObject( 6, x3 + 745, -1 );
    Obstacle tmp3;
    tmp3.setup( ofVec2f( x3 + 2000, ofGetHeight() ), 100, 150, true );
    obstacleList.push_back( tmp3 );
    
    // Spring off notes to get to higher notes and overcome a tall obstacle.
    float x4 = 4600;
    addObject( 2, x4, -1 );
    //    addObject( 5, x4 + 200, -1 );
    addObject( 8, x4 + 400, -1 );
    addObject( 5, x4 + 700, -1 );
    float x5 = x4 + 1100;
    addObject( 5, x5, -1 );
    addObject( 2, x5 + 100, -1 );
    //    addObject( 8, x5 + 200, -1 );
    Obstacle tmp4;
    tmp4.setup( ofVec2f( x5 + 400, ofGetHeight() ), 100, 300, true );
    obstacleList.push_back( tmp4 );
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
            if ( gameState == 1 ) {
                gameState = -1;
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
                gameState = 1;
            }
            break;
            
            // Proceed on menu.
        case OF_KEY_RETURN:
            if ( gameState == 0 ) gameState = 1;
            break;
            
            //----------------------------------------------------
            // Movement and action (depends on the control scheme).
            
            // UP
        case 'w':
        case 'W':
            if ( bIsLefty ) {}
            else myPlayer.up = true;
            break;
        case OF_KEY_UP:
            if ( gameState == 0 ) {
                bIsLefty = !bIsLefty;
            } else {
                if ( bIsLefty ) myPlayer.up = true;
                else {}
            }
            break;
            
            // LEFT
        case 'a':
        case 'A':
            if ( bIsLefty ) myPlayer.record = true;
            else myPlayer.left = true;
            break;
        case OF_KEY_LEFT:
            if ( bIsLefty ) myPlayer.left = true;
            else myPlayer.record = true;
            break;
            
            // DOWN
        case 's':
        case 'S':
            if ( bIsLefty ) {}
            else myPlayer.down = true;
            break;
        case OF_KEY_DOWN:
            if ( gameState == 0 ) {
                bIsLefty = !bIsLefty;
            } else {
                if ( bIsLefty ) myPlayer.down = true;
                else {}
            }
            break;
            
            // RIGHT
        case 'd':
        case 'D':
            if ( bIsLefty ) {
                myPlayer.replay = true;
                if ( myPlayer.bAllowReplay ) fReplay();
            }
            else myPlayer.right = true;
            break;
        case OF_KEY_RIGHT:
            if ( bIsLefty ) myPlayer.right = true;
            else {
                myPlayer.replay = true;
                if ( myPlayer.bAllowReplay ) fReplay();
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
            
            //----------------------------------------------------
            // Movement and action (depends on the control scheme).
            
            // UP
        case 'w':
        case 'W':
            if ( bIsLefty ) {}
            else {
                myPlayer.up = false;
                myPlayer.allowJump = true;
            }
            break;
        case OF_KEY_UP:
            if ( bIsLefty ) {
                myPlayer.up = false;
                myPlayer.allowJump = true;
            }
            else {}
            break;
            
            // LEFT
        case 'a':
        case 'A':
            if ( bIsLefty ) myPlayer.record = false;
            else myPlayer.left = false;
            break;
        case OF_KEY_LEFT:
            if ( bIsLefty ) myPlayer.left = false;
            else myPlayer.record = false;
            break;
            
            // DOWN
        case 's':
        case 'S':
            if ( bIsLefty ) {}
            else myPlayer.down = false;
            break;
        case OF_KEY_DOWN:
            if ( bIsLefty ) myPlayer.down = false;
            else {}
            break;
            
            // RIGHT
        case 'd':
        case 'D':
            if ( bIsLefty ) myPlayer.replay = false;
            else myPlayer.right = false;
            break;
        case OF_KEY_RIGHT:
            if ( bIsLefty ) myPlayer.right = false;
            else myPlayer.replay = false;
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
