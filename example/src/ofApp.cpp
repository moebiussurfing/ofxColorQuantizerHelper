#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);

	ofSetWindowPosition(-1080, 25);
	ofSetWindowShape(1080, 1920);

	ui.setup();

	colorQuantizer.setUiPtr(&ui);
	colorQuantizer.setEnableNativeWidgets(false);
	colorQuantizer.setup();

	// reference first or clicked palette color
	colorQuantizer.setColor_BACK(c);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(c);

	colorQuantizer.draw();

	ui.Begin();
	{
		if (ui.BeginWindow("ofApp"))
		{
			ui.Add(colorQuantizer.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
			if (colorQuantizer.bGui) {
				ui.Add(colorQuantizer.bGui_Picture, OFX_IM_TOGGLE_ROUNDED);
				ui.Add(colorQuantizer.bGui_Library, OFX_IM_TOGGLE_ROUNDED);
			}
			ui.EndWindow();
		}

		colorQuantizer.drawImGuiWindows();
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
}
