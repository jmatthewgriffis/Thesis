//
//  Staff.h
//  Prototype_6_controller_and_clean_code
//
//  Created by J. Matthew Griffis on 2/27/14.
//
//

#pragma once

#include "ofMain.h"

class Staff {
public:
    Staff();
    
    void update();
    void draw( int iScaler, float fMeasureLength, int gameState, ofTrueTypeFont helvetica, ofTrueTypeFont helveticaJumbo, float myPlayerX );
    
    int iStaffAlphaMin, iStaffAlphaMax;
    
    float iStaffAlpha, iStaffAlphaVel;
    
    bool bDrawStaffLines, bDrawStaffBracketAndClefs, bDrawMeasureLines, bDrawMeasureNumbers, bHideAll;
    
    ofImage trebleClef, bassClef, staffBracket;
};