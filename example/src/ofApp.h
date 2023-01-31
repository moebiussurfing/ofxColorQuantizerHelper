#pragma once

#include "ofMain.h"

#include "ofxColorQuantizerHelper.h"
#include "ofxSurfingImGui.h"

class ofApp: public ofBaseApp
{
	public:
		void setup();
		void draw();
		void keyPressed(int key);
		
		ofxColorQuantizerHelper colorQuantizer;

		// NOTE
		// This addon requires to instantiate ImGui to "draw" widgets panels inside.
		// the addon itself has th
		ofxSurfingGui ui;
};
