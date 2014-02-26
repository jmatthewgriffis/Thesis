//
//  Player.cpp
//  Prototype_2_with_notes
//
//  Created by J. Matthew Griffis on 10/26/13.
//
//

#include "Player.h"

Player::Player() {
    
    capacity = CAPACITY;
    fHealthMax = 100;
    fHealthLossSpeed = 0.25;
    
    headphones.loadImage( "images/headphones.png" );
    hand.loadImage( "images/hand.png" ); // http://upload.wikimedia.org/wikipedia/commons/f/fa/Hand.svg
}

void Player::setup( int _iScaler, bool _bUsingController, ofVec2f _pos ) {
    
    iScaler = _iScaler;
    bUsingController = _bUsingController;
    
    wide = iScaler * 2;
    tall = wide;
    maxVel = float( iScaler / 3.4483 ); // Yields 120bpm.
    jumpVel = iScaler / 2.5;
    radius = iScaler * 1.6;
    //angleVel = float( iScaler / 1.6667 );
    angleVel = 15;
    
    up = left = down = right = onSurface = record = replay = bIsActing = bIsRecording = bIsReplaying = bIsEmpty = bIsFull = false;
    allowMove = true;
    allowJump = bAllowRecord = bAllowReplay = true;
    angle = 0;
    fHealth = fHealthMax;
    
    pos.set( _pos );
    vel.set( 0 );
    acc.set( 0 );
    actPos.set( 0 );
}

void Player::update( int _gameState ) {
    
    gameState = _gameState;
    
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
    
    // Movement
    if ( allowMove ) {
        
        if ( up ) {
            if ( gameState >= 3 ) {
                applyForce( ofVec2f( 0.0, -maxVel ) );
            } else if ( onSurface && allowJump ) {
                    //            pos.y -= vel.y;
                    onSurface = false;
                    allowJump = false;
                    // Jump! And prevent additional jumps.
                    applyForce( ofVec2f( 0.0, -jumpVel ) );
                    }
        }
        if ( down ) {
            if ( gameState >= 3 ) {
                //            pos.y += vel.y;
                applyForce( ofVec2f( 0.0, maxVel ) );
            }
        }
        
        if ( left ) {
            //if ( gameState < 3 ) {
                applyForce( ofVec2f( -maxVel, 0.0 ) );
            //}
        }
        if ( right ) {
            //if ( gameState < 3 ) {
                applyForce( ofVec2f( maxVel, 0.0 ) );
            //}
        }
        
        vel += acc;
        
        // Negate velocity on a surface.
        if ( onSurface ) {
            vel.y = 0;
        }
        
        pos += vel;
        
        if ( gameState >= 3 ) {
            pos.x += maxVel;
        }
        
        if ( !bUsingController ) {
            //        if ( onSurface ) {
            vel.x = 0;
            if ( gameState == 3 ) {
                vel.y = 0;
            }
            //        }
        }
    } // End "if allowMove"
    
    // Prevent going offscreen.
    if ( pos.x <= wide / 2.0 ) {
        pos.x = wide / 2.0;
    }
    if ( pos.y <= tall / 2.0 ) {
        pos.y = tall / 2.0;
    }
    else if ( pos.y >= ofGetHeight() - ( tall / 2.0 ) ) {
        pos.y = ofGetHeight() - ( tall / 2.0 );
    }
    
    // Manage forces.
    if ( gameState < 3 ) {
        float damping = 0.97;
        vel.y *= damping;
        //vel.y *= damping / 2.0;
        vel.x *= damping / 2.0;
    }
    acc.set( 0 );
    
    fPressingRecord();
    fPressingReplay();
    fActing();
}

void Player::draw( ofTrueTypeFont _font, vector< Object > _recordedList ) {
    
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
    if ( _recordedList.size() > 0 ) {
        
        ofSetRectMode( OF_RECTMODE_CENTER );
        ofNoFill();
        ofRect( pos.x, _recordedList[ 0 ].pos.y, _recordedList[ 0 ].wide, _recordedList[ 0 ].tall );
        ofFill();
    }
    
    // Draw the player.
    ofSetRectMode( OF_RECTMODE_CENTER );
    ofSetColor( 0 );
    ofRect( pos, wide, tall );
    
    // Draw the health. Taken from my Space Odyssey 2 code.
    ofPushMatrix();{
        
        ofTranslate( pos.x, pos.y);
        
        /*
         {
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
         }
         */
        
    }ofPopMatrix();
    
    ofSetColor( 255 );
    ofSetRectMode( OF_RECTMODE_CORNER );
    headphones.draw( pos.x-iScaler*1.4, pos.y-iScaler*2.4, iScaler * 3, iScaler * 3 );
    //hand.draw( pos.x - wide / 2, pos.y - tall / 2, wide, tall );
    
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

void Player::applyForce( ofVec2f _force ) {
    
    acc += _force;
}

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