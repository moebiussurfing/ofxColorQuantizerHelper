#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);

	colorQuantizer.setup();
	guiManager.setup();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(100, 100, 100);

	colorQuantizer.draw();

	guiManager.begin();
	{
		colorQuantizer.draw_Gui();
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'G') colorQuantizer.setToggleActive();
}
