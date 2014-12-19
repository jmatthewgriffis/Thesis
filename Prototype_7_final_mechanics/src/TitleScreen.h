//
//  TitleScreen.h
//  Prototype_6_controller_and_new_mechanic
//
//  Created by J. Matthew Griffis on 2/24/14.
//
//

#pragma once

#include "ofMain.h"

class TitleScreen {
public:
    TitleScreen();
    
    void draw( int iScaler, ofTrueTypeFont _font, bool bIsLefty, bool bUsingController );
    
    bool bChoseControls;
    
    int iWhichPrototype;
};