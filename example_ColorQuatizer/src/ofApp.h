#pragma once

#include "ofMain.h"

#include "ofxColorQuantizerHelper.h"
#include "ofxSurfingImGui.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void draw();
	void keyPressed(int key);

	ofxColorQuantizerHelper colorQuantizer;

	// NOTE
	// This add-on requires to instantiate and pass as reference the ImGui object
	// to "draw" widgets panels inside.
	ofxSurfingGui ui;

	ofColor c;

	ofImage img;
	bool bDrawImg = true;
};