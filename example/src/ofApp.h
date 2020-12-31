#pragma once

#include "ofMain.h"
#include "ofxColorQuantizerHelper.h"

class ofApp: public ofBaseApp{
	public:
		void setup();
		void draw();
		void keyPressed(int key);
		
		ofxColorQuantizerHelper colorQuantizer;
};
