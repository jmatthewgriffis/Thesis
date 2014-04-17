//
//  Player.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Player.h"

//--------------------------------------------------------------
Player::Player() {
    
    capacity = CAPACITY;
    fHealthMax = 100;
    fHealthLossSpeed = 0.25;
    
    headphones.loadImage( "images/headphones.png" );
    hand.loadImage( "images/hand.png" ); // http://upload.wikimedia.org/wikipedia/commons/f/fa/Hand.svg
    appendage.loadImage("images/player/eighth-rest.png"); // http://www.clker.com/cliparts/w/1/j/6/b/y/eighth-rest.svg
    appendage_mirrored.loadImage("images/player/eighth-rest_mirrored.png");
    hat.loadImage("images/player/half-rest.png"); // https://teacher.ocps.net/karen.doss/media/halfrest.png
}

//--------------------------------------------------------------
void Player::setup( int _gameState, int _frameRate, int _iScaler, bool _bUsingController, ofVec2f _pos, vector< float > _staffPosList ) {
    
    gameState = _gameState;
    frameRate = _frameRate;
    iScaler = _iScaler;
    bUsingController = _bUsingController;
    
    staffPosList = _staffPosList;
    myObject.setup(iScaler, staffPosList, "c3_middle", 1, -1);
    
    if (gameState < 7) {
        wide = iScaler * 2;
    } else {
        wide = iScaler * 1.5;
    }
    tall = wide;
    if (gameState == 8) {
        maxVel = float( iScaler / 3.4483 ) * 0.75; // Yields 90bpm.
    } else {
        maxVel = float( iScaler / 3.4483 ); // Yields 120bpm.
    }
    jumpVel = -iScaler * 0.45;
    fHatSizer = wide * 0.0131;
    fHatWidth = hat.getWidth() * fHatSizer;
    fHatHeight = hat.getHeight() * fHatSizer;
    fHatOffsetDefault = pos.y - fHatHeight * 0.5 + wide * -0.65;
    fHatOffset = fHatOffsetDefault;
    fHatVelDefault = iScaler * 0.0095;
    fHatVel = 0;
    fHatQueuedForce = 0;
    radius = iScaler * 1.6;
    //angleVel = float( iScaler / 1.6667 );
    angleVel = 15;
    fNoteOffsetH = 0;
    currentStream = -1;
    inStreamTimer = invisibleTimer = 0;
    jumpCounter = 1;
    
    up = left = down = right = onSurface = onStream = record = replay = bIsActing = bIsRecording = bIsReplaying = bIsEmpty = bIsFull = bModePlatformer = bModeSurf = bModeFlight = bIsOnlyOneRoom = bCanMakeNotes = bAutoplayBass = closeEnough = bGrabHat = bFlyingHat = bNoteFlyingHatAngle = onStreamPrev = false;
    allowMove = true;
    allowControl = true;
    allowJump = bAllowRecord = bAllowReplay = drawPlayer = true;
    bHasShip = false;
    angle = myAngle = hatAngle = 0;
    fHealth = fHealthMax;
    velPrev = 0;
    
    pos.set( _pos );
    yPosLast = pos.y;
    yPosDiff = 0;
    vel.set( 0 );
    acc.set( 0 );
    actPos.set( 0 );
    
    //------------
    
    if ( gameState == 1 ) { // Tutorial
        bModePlatformer = true;
        
    } else if ( gameState == 2 ) { // Boss
        bModePlatformer = true;
        bIsOnlyOneRoom = true;
        
    } else if (gameState == 3) { // Piano groove
        bModeFlight = true;
        
    } else if (gameState == 4) { // Flight!
        bModeSurf = true;
        bAutoplayBass = true;
        
    } else if (gameState == 5) { // Solo!
        bModeFlight = true;
        bCanMakeNotes = true;
        bAutoplayBass = true;
        
    } else if (gameState == 6) { // Solo--gym
        bModeFlight = true;
        bCanMakeNotes = true;
        bIsOnlyOneRoom = true;
        
    } else if (gameState == 7 || gameState == 8) { // Surfin' USA
        bModeSurf = true;
        bHasShip = true;
        bAutoplayBass = true;
        
    }
    
    
    if (bHasShip) {
        myShip.setup();
    }
    
    //------------
}

//--------------------------------------------------------------
void Player::update( int _gameState, string _OnThisNote ) {
    
    gameState = _gameState;
    
    yPosLast = pos.y;
    
    yPosStaff = myObject.fReturnYPos(_OnThisNote);
    
    /*
     // Health depletes constantly.
     if ( fHealth > fHealthMax ) {
     fHealth = fHealthMax;
     }
     else if ( fHealth > 0 ) {
     fHealth -= fHealthLossSpeed;
     }
     else if ( fHealth < 0 ) {
     fHealth = 0;
     }
     */
    
    if (!drawPlayer) {
        invisibleTimer++;
    }
    if (invisibleTimer >= 10) {
        drawPlayer = true;
        invisibleTimer = 0;
    }
    
    if (gameState >= 7 && bModeSurf) {
        if (myShip.onStream && vel.y >= 0) {
            jumpCounter = 0;
        } else if (!myShip.onStream && jumpCounter < 1) {
            jumpCounter = 1;
        }
    }
    
    // Prevent going off the true left and bottom edges.
    if ( pos.x <= wide / 2.0 ) {
        pos.x = wide / 2.0;
    }
    if ( pos.y >= ofGetHeight() - ( tall / 2.0 ) ) {
        pos.y = ofGetHeight() - ( tall / 2.0 );
    }
    
    // Prevent from going offscreen to the left when zoomed in.
    if ( pos.x < iScaler * 7 ) {
        pos.x = iScaler * 7;
    }
    // Prevent from going offscreen to the right when zoomed in.
    if ( bIsOnlyOneRoom ) {
        if ( pos.x > ofGetWidth() - iScaler * 7 ) {
            pos.x = ofGetWidth() - iScaler * 7;
        }
    }
    
    // Movement
    
    // Move forward constantly.
    if ((bModeSurf || bModeFlight) && gameState != 6 && allowMove) {
        if ( (gameState == 4 && onSurface == true) /*|| (gameState != 4 && bModeSurf && onStream && !closeEnough)*/ ) {
            pos.x += maxVel * 0.25f;
        } else {
            pos.x += maxVel;
        }
    }
    
    if ( allowMove ) {
        
        if (allowControl) {
            // Up-----------
            if ( up ) {
                if ( bModePlatformer ) {
                    
                    if ( onSurface && allowJump ) {
                        applyForce( ofVec2f( 0.0, jumpVel ) );
                        onSurface = false;
                        allowJump = false;
                    }
                    
                } else if ( bModeFlight ) {
                    applyForce( ofVec2f( 0.0, -maxVel ) );
                    
                } else if ( bModeSurf ) {
                    
                    if ( allowJump == true ) {
                        vel.y = 0;
                        applyForce( ofVec2f( 0.0, jumpVel * 0.75 ) );
                        onSurface = false;
                        onStream = false;
                        allowJump = false; // yo yo
                        if (gameState >= 7) {
                            jumpCounter++;
                        }
                    }
                }
            } else if ( bModeSurf && !down ){
                allowJump = true;
            }
            
            // Down-----------
            if ( down ) {
                if ( bModeFlight ) {
                    //            pos.y += vel.y;
                    applyForce( ofVec2f( 0.0, maxVel ) );
                } else if (bModeSurf) {
                    if ( allowJump == true ) {
                        vel.y = 0;
                        applyForce( ofVec2f( 0.0, -jumpVel * 0.4 ) );
                        //onSurface = false;
                        allowJump = false;
                    }
                }
            } else if (bModeSurf && !up) {
                allowJump = true;
            }
            
            // Left-----------
            if ( left ) {
                //if ( gameState < 3 ) {
                applyForce( ofVec2f( -maxVel, 0.0 ) );
                //}
            }
            
            // Right-----------
            if ( right ) {
                //if ( gameState < 3 ) {
                applyForce( ofVec2f( maxVel, 0.0 ) );
                //}
            }
        }
        
        // Limit jumping.
        if (bModeSurf && gameState >= 7 && jumpCounter >= 2) {
            allowJump = false;
        }
        
        vel += acc;
        
        if (!onStreamPrev && onStream) {
            velPrev = vel.y;
        }
        
        // Negate velocity on a surface.
        if ( onSurface || onStream ) {
            vel.y = 0;
        }
        
        pos += vel;
        
        if ( bModePlatformer || gameState == 6 ) {
            vel.x = 0;
        }
        
        if ( bModeFlight ) {
            //if ( bUsingController == false ) {
            vel.y = 0;
            //}
            //        }
        }
    } // End "if allowMove"
    
    { // Update hat. Hat has its own physics.
        fHatOffsetDefault = pos.y - fHatHeight * 0.5 + wide * -0.65;
        
        // Build up force as the player rises then make the hat pop into the air when the player stops or reverses direction.
        if (yPosDiff < 0 && !bGrabHat) {
            if (pos.y - yPosLast < 0) {
                fHatQueuedForce += yPosDiff * 0.025;
                // Add terminal upward vel to keep this under control.
                float limit = -wide / 5;
                if (fHatQueuedForce < limit) {
                    fHatQueuedForce = limit;
                }
            } else {
                fHatVel += fHatQueuedForce;
                fHatQueuedForce = 0;
            }
        }
        
        if (fHatOffset < fHatOffsetDefault) {
            fHatVel += fHatVelDefault;
            if (!bGrabHat) {
                bFlyingHat = true;
            }
        } else if (fHatOffset >= fHatOffsetDefault) {
            fHatOffset = fHatOffsetDefault;
            fHatVel = 0;
            bFlyingHat = false;
            bNoteFlyingHatAngle = false;
        }
        
        if (inStreamTimer > 0 && bGrabHat && fHatOffset >= fHatOffsetDefault - iScaler) {
            fHatOffset = fHatOffsetDefault;
        } else if (fHatOffset < fHatOffsetDefault - iScaler && !bFlyingHat) {
            fHatOffset = fHatOffsetDefault;
        } else {
            fHatOffset += fHatVel;
        }
    } // End updating hat.
    
    yPosDiff = pos.y - yPosLast;
    
    // Manage forces.
    if ( bModePlatformer ) {
        float damping = 0.97;
        vel.y *= damping;
        //vel.y *= damping / 2.0;
        vel.x *= damping / 2.0;
    }
    acc.set( 0 );
    
    fPressingRecord();
    fPressingReplay();
    fActing();
    
    if (bHasShip) {
        myShip.update(ofVec2f(pos.x + wide * 0.45, pos.y + tall * 1.1), tall, allowControl, jumpCounter);
    }
    
    if (!allowControl) {
        if (myShip.angle < 180) {
            myShip.angle += 0.5;
        } else if (myShip.angle > 180) {
            myShip.angle -= 0.5;
        }
    }
    
    // Use a timer to keep the rider in the stream.
    float timeLimit = frameRate * 0.25;
    if (onStream) {
        inStreamTimer = timeLimit;
    }
    if (inStreamTimer > 0) {
        if (vel.y < 0) { // Jump to leave the stream.
            inStreamTimer = 0;
        } else {
            inStreamTimer--;
            myShip.onStream = true;
        }
    } else {
        myShip.onStream = false;
    }
    onStreamPrev = onStream;
}

//--------------------------------------------------------------
void Player::draw( ofTrueTypeFont _font, vector< Object > _recordedList ) {
    
    /*ofSetColor(0);
    ofDrawBitmapString(ofToString(jumpCounter), pos.x + 100, pos.y);*/
    
    fDrawRecordedList(_recordedList);
    
    // Draw the generic player for the early prototypes.
    ofSetRectMode( OF_RECTMODE_CENTER );
    ofSetColor( 0 );
    if ( gameState != 3 && gameState < 7 ) {
        ofRect( pos, wide, tall );
    }
    
    // Draw the hands for Piano groove.
    ofSetColor( 255 );
    ofSetRectMode( OF_RECTMODE_CORNER );
    if ( gameState == 3 ) {
        //fDrawHealth();
        wide = iScaler * 4;
        tall = wide;
        hand.draw( pos.x - wide / 2, pos.y - tall / 2, wide, tall);
    }
    
    /*
     // Draw the headphones for Flight!.
     if ( gameState >= 4 ) {
     headphones.draw( pos.x-iScaler*1.4, pos.y-iScaler*2.4, iScaler * 3, iScaler * 3 );
     }
     */
    // Draw wings for Flight!.
    if ( gameState == 4 ) {
        ofPushMatrix();{
            ofTranslate( pos.x, pos.y );
            if ( vel.y < 0 ) {
                ofRotate( -30 );
            } else if ( vel.y > 0 ) {
                ofRotate( 30 );
            } else {
                ofRotate( 0 );
            }
            ofSetColor(0, 255);
            ofSetLineWidth( 3 );
            //ofLine( 0, 0, - iScaler * 3, -iScaler * 0.5 );
            ofLine( 0, 0, - iScaler * 3, 0 );
            ofSetLineWidth( 1 );
        }ofPopMatrix();
    }
    
    // Draw the actual character now that he exists.
    if (gameState >= 7 && drawPlayer) {
        fDrawCharacter();
    }
    
    fDrawCapacity(_recordedList);
    fDrawAction(_font);
    
    if (bHasShip && drawPlayer) {
        myShip.draw();
    }
}

//--------------------------------------------------------------
void Player::applyForce( ofVec2f _force ) {
    
    acc += _force;
}

//--------------------------------------------------------------
void Player::fPressingRecord() {
    
    // Player is pressing the record key.
    if ( record ) {
        // Make sure not already acting and/or holding the key down.
        if ( !bIsActing && bAllowRecord ) {
            // Trigger the action. Prevent additional calls while this one is going.
            angle = 225;
            bIsActing = true;
            bIsRecording = true;
            bAllowRecord = false;
        }
    } else if ( !bIsActing ) {
        // If not already pressing the key AND not acting, enable recording.
        bAllowRecord = true;
    }
}

//--------------------------------------------------------------
void Player::fPressingReplay() {
    
    // Player is pressing the replay key.
    if ( replay ) {
        // Make sure not already acting and/or holding the key down.
        if ( !bIsActing && bAllowReplay ) {
            // Trigger the action. Prevent additional calls while this one is going.
            angle = 315;
            bIsActing = true;
            bIsReplaying = true;
            bAllowReplay = false;
        }
    } else if ( !bIsActing ) {
        // If not already pressing the key AND not acting, enable recording.
        bAllowReplay = true;
    }
}

//--------------------------------------------------------------
void Player::fActing() {
    
    if ( bIsRecording ) {
        angle -= angleVel;
        if ( angle < -135 ) {
            bIsRecording = false;
        }
    } else if ( bIsReplaying ) {
        angle += angleVel;
        if ( angle > 675 ) {
            bIsReplaying = false;
        }
    } else {
        bIsActing = false;
    }
    
    // Calculate the position of the action circle.
    if ( bIsActing ) {
        actPos.x = pos.x + radius * sin( ofDegToRad( angle ) );
        actPos.y = pos.y + radius * cos( ofDegToRad( angle ) );
    }
}

//--------------------------------------------------------------
void Player::fDrawRecordedList(vector< Object > _recordedList) {
    // We want to do something when the vector is empty, but not during the action that empties it, so we use a boolean that activates only after that last action is complete.
    if ( _recordedList.size() > 0 ) {
        bIsEmpty = false;
    } else if ( !bIsActing ) {
        bIsEmpty = true;
    }
    // We want to do something when the vector is full, but not during the action that fills it, so we use a boolean that activates only after that last action is complete.
    if ( _recordedList.size() < capacity ) {
        bIsFull = false;
    } else if ( !bIsActing ) {
        bIsFull = true;
    }
    
    // Display an outline of the next replayable note (drawn from the center).
    if (gameState < 5) {
        if ( _recordedList.size() > 0 ) {
            
            ofSetRectMode( OF_RECTMODE_CENTER );
            ofSetColor( 0, 255 );
            ofNoFill();
            ofEllipse( pos.x, _recordedList[ 0 ].pos.y, _recordedList[ 0 ].wide, _recordedList[ 0 ].tall );
            ofFill();
        }
    } else {
        if (allowMove && bCanMakeNotes) {
            ofPushMatrix();{
                float tmpX;
                if (bHasShip) {
                    fNoteOffsetH = myShip.fImgHeightBass * 1.75;
                    tmpX = myShip.pos.x;
                } else {
                    fNoteOffsetH = wide * 1.25;
                    tmpX = pos.x;
                }
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofSetColor(0, 255);
                ofNoFill();
                ofEllipse(tmpX - fNoteOffsetH, yPosStaff, myObject.wide, myObject.tall);
                ofFill();
            }ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void Player::fDrawCapacity(vector< Object > _recordedList) {
    // Display a visual indicator of recorded capacity.
    if ( _recordedList.size() > 1 ) {
        for ( int i = 0; i < _recordedList.size(); i++ ) {
            float rad, tmpPosX, tmpPosY, hOffset, vOffset;
            rad = 3;
            //hOffset = ( wide - ( rad * 3 ) ) / 4;
            hOffset = wide / 4;
            vOffset = tall / 4;
            if ( i < 3 ) {
                tmpPosX = pos.x - wide / 2 + hOffset + hOffset * i;
                tmpPosY = pos.y - tall / 2 + vOffset;
            } else if ( i >= 3 && i < 6 ) {
                tmpPosX = pos.x - wide / 2 + hOffset + hOffset * ( i - 3 );
                tmpPosY = pos.y - tall / 2 + vOffset * 2;
            } else if ( i >= 6 && i < 9 ) {
                tmpPosX = pos.x - wide / 2 + hOffset + hOffset * ( i - 6 );
                tmpPosY = pos.y - tall / 2 + vOffset * 3;
            }
            ofNoFill();
            ofSetColor( 255 );
            ofCircle( tmpPosX, tmpPosY, rad );
            ofFill();
        }
    }
}

//--------------------------------------------------------------
void Player::fDrawAction(ofTrueTypeFont _font) {
    // Draw the action if called, orbiting around the player's pos.
    if ( bIsActing ) {
        if ( bIsRecording ) {
            // Feedback for no capacity.
            if ( bIsFull ) {
                ofSetColor( 0 );
                _font.drawString("X", pos.x + float( iScaler / 0.8333 ), pos.y - float( iScaler / 0.8333 ) );
            }
            ofSetColor( 0, 255, 0 );
        }
        else if ( bIsReplaying ) {
            // Feedback for nothing to replay.
            if ( bIsEmpty ) {
                ofSetColor( 0 );
                _font.drawString("?", pos.x + float( iScaler / 0.8333 ), pos.y - float( iScaler / 0.8333 ) );
            }
            ofSetColor( 0, 0, 255 );
        }
        ofCircle( actPos, iScaler / 2.5 );
    }
}

//--------------------------------------------------------------
void Player::fDrawHealth() {
    // Draw the health. Taken from my Space Odyssey 2 code.
    ofPushMatrix();{
        
        ofTranslate( pos.x, pos.y);
        
        // Draw the health bar
        ofSetRectMode( OF_RECTMODE_CORNER );
        float offset = 1;
        float offsetBar = iScaler / 2.5;
        float barHeight = iScaler / 2.5;
        float barLength = wide * 2;
        float currentHealth = ofMap( fHealth, 0, fHealthMax, 0, barLength - offset * 2 );
        // The border.
        ofSetColor( 255 );
        ofNoFill();
        ofRect( -wide, wide / 2 + offsetBar, barLength, barHeight );
        ofFill();
        // The current health.
        ofSetColor( 0, 255, 0 );
        ofRect( -wide + offset, wide / 2 + offsetBar + offset, currentHealth, barHeight - offset * 2 );
        
    }ofPopMatrix();
}

//--------------------------------------------------------------
void Player::fDrawCharacter() {
    
    // Draw variable value for debugging.
    /*string str;
     if (bFlyingHat) str = "true";
     else str = "false";
     ofSetColor(0);
     ofDrawBitmapString(ofToString(myShip.angle), pos.x + 100, pos.y - 50);*/
    
    tall = wide * 1.35;
    bGrabHat = false;
    
    ofPushMatrix();{
        float newAngle;
        float lowerLimit = 50;
        float upperLimit = 295;
        if (bHasShip) {
            if ((myShip.angle <= lowerLimit && myShip.clockwise) || (myShip.angle >= upperLimit && !myShip.clockwise)) {
                myAngle = myShip.angle;
            } else {
                bGrabHat = true;
            }
            newAngle = myShip.angle;
        }
        
        ofTranslate(myShip.pos);
        ofRotate(newAngle);
        
        ofPushMatrix();{
            
            ofTranslate(-myShip.pos);
            
            // Hat
            /*ofSetColor(255, 255);
             ofSetRectMode(OF_RECTMODE_CORNER);
             ofPushMatrix();{
             ofTranslate(pos.x - fHatWidth * 0.5, fHatOffset);
             ofRotate(-10);
             hat.draw(-wide * 0.5 * 0.25, 0, fHatWidth, fHatHeight);
             }ofPopMatrix();*/
            
            // Body and hat
            ofPushMatrix();{
                ofTranslate(pos);
                ofRotate(-myAngle);
                // Body
                ofSetColor(0, 255);
                ofEllipse(0, 0, wide, tall);
                // Hat
                ofSetColor(255, 255);
                ofSetRectMode(OF_RECTMODE_CORNER);
                ofPushMatrix();{
                    /*if (bFlyingHat) { // Find me
                     if (!bNoteFlyingHatAngle) {
                     hatAngle = myAngle;
                     bNoteFlyingHatAngle = true;
                     }
                     float diff = hatAngle - myAngle;
                     float counterRot;
                     if (diff != 0) {
                     if (hatAngle > myAngle) {
                     counterRot = -diff;
                     } else {
                     counterRot = diff;
                     }
                     }
                     //ofRotate(counterRot); // Find me
                     }*/
                    ofPushMatrix();{
                        ofTranslate(-fHatWidth * 0.5, fHatOffset - pos.y);
                        ofRotate(-10);
                        //if (bFlyingHat && bGrabHat){
                        // Find me
                        //} else {
                        hat.draw(-wide * 0.5 * 0.25, 0, fHatWidth, fHatHeight);
                        //}
                    }ofPopMatrix();
                }ofPopMatrix();
                //
                ofSetColor(255, 255);
                ofSetRectMode(OF_RECTMODE_CORNER);
                float armSizer = wide * 0.00286;
                float armWidth = appendage.getWidth() * armSizer;
                float armHeight = appendage.getHeight() * armSizer;
                // Right arm
                ofPushMatrix();{
                    ofTranslate(wide * 0.35, -wide * 0.15);
                    float addition;
                    if (myShip.clockwise && myAngle != newAngle) {
                        addition = -15;
                    } else {
                        addition = 0;
                    }
                    ofRotate(55 + addition);
                    appendage.draw(-armWidth * 0.4, -armHeight, armWidth, armHeight);
                    // Test circle
                    /*ofSetColor(255,0,0);
                     ofCircle(0,0,5);*/
                }ofPopMatrix();
                // Left arm
                ofPushMatrix();{
                    ofTranslate(-wide * 0.35, -wide * 0.15);
                    float addition;
                    if (!myShip.clockwise && myAngle != newAngle) {
                        addition = 40;
                    } else {
                        addition = 0;
                    }
                    ofRotate(-70 + addition);
                    appendage_mirrored.draw(-armWidth * 0.6, -armHeight, armWidth, armHeight);
                    // Test circle
                    /*ofSetColor(255,0,0);
                     ofCircle(0,0,5);*/
                }ofPopMatrix();
            }ofPopMatrix();
            
            // Appendages
            ofSetColor(255, 255);
            ofSetRectMode(OF_RECTMODE_CORNER);
            float armSizer = wide * 0.00286;
            float armWidth = appendage.getWidth() * armSizer;
            float armHeight = appendage.getHeight() * armSizer;
            // Right arm
            /*ofPushMatrix();{
             ofTranslate(pos.x + wide * 0.35, pos.y - wide * 0.15);
             ofRotate(55 - myAngle * 2);
             appendage.draw(-armWidth * 0.4, -armHeight, armWidth, armHeight);
             // Test circle
             ofSetColor(255,0,0);
             ofCircle(0,0,5);
             }ofPopMatrix();*/
            // Right leg
            ofPushMatrix();{
                ofTranslate(pos.x + wide * 0.25, pos.y + wide * 0.45);
                ofRotate(125);
                appendage.draw(-armWidth * 0.4, -armHeight, armWidth, armHeight);
            }ofPopMatrix();
            // Left arm
            /*ofPushMatrix();{
             ofTranslate(pos.x - wide * 0.35, pos.y - wide * 0.15);
             ofRotate(-70 - myAngle * 2);
             appendage_mirrored.draw(-armWidth * 0.6, -armHeight, armWidth, armHeight);
             // Test circle
             ofSetColor(255,0,0);
             ofCircle(0,0,5);
             }ofPopMatrix();*/
            // Left leg
            ofPushMatrix();{
                ofTranslate(pos.x - wide * 0.25, pos.y + wide * 0.45);
                ofRotate(-135);
                appendage_mirrored.draw(-armWidth * 0.6, -armHeight, armWidth, armHeight);
            }ofPopMatrix();
        }ofPopMatrix();
    }ofPopMatrix();
    // Test circle
    /*ofSetColor(0,0,255);
     ofCircle(pos, 5);*/
    // Test lines
    /*ofLine(myShip.pointFront, pos);
     ofLine(myShip.pointRear, pos);*/
}