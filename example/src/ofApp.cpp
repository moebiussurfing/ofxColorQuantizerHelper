#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetWindowPosition(-1920, 25);

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
			ui.Add(colorQuantizer.bGui_Library, OFX_IM_TOGGLE_ROUNDED);
			ui.Add(colorQuantizer.bGui_Picture, OFX_IM_TOGGLE_ROUNDED);
			ui.AddSpacingBigSeparated();
			ui.AddMinimizerToggle();
			if (ui.isMaximized()) {
				ui.Add(colorQuantizer.bGui_Advanced, OFX_IM_TOGGLE_ROUNDED_MINI);
				ui.Add(colorQuantizer.bGui_InfoHelp, OFX_IM_TOGGLE_ROUNDED_MINI);
				ui.Add(colorQuantizer.bGui_InfoImage, OFX_IM_TOGGLE_ROUNDED_MINI);
				ui.Add(colorQuantizer.getUiPtr()->bAutoResize, OFX_IM_TOGGLE_ROUNDED_MINI);
			}
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
