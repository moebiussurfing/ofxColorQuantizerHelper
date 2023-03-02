#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() 
{
	ofSetFrameRate(60);

	ofSetWindowPosition(-1080, 25);
	ofSetWindowShape(1080, 1920);

	ui.setup();

	colorQuantizer.setUiPtr(&ui);
	colorQuantizer.setup();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(100, 100, 100);

	colorQuantizer.draw();

	ui.Begin();
	{
		if (ui.BeginWindow("ofApp"))
		{
			ui.Add(colorQuantizer.bGui_Picture, OFX_IM_TOGGLE_ROUNDED);
			ui.Add(colorQuantizer.bGui_Library, OFX_IM_TOGGLE_ROUNDED);
			ui.EndWindow();
		}

		colorQuantizer.draw_ImGuiWidgets();
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'G') colorQuantizer.setToggleActive();
}
