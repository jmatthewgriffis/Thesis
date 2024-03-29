#pragma once

#include "ofMain.h"
#include "Note.h"

#define numNotes 15

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
		
    // Store all the y-pos of the notes on the musical staff.
    vector< float > staffPosList;
    // Use a function to define all of them.
    void staffPosSet();
    
    // Store all the notes.
    vector< Note > noteList;
};
