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

	// reference first or clicked palette color
	colorQuantizer.setColor_BACK(c);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(c);

	//colorQuantizer.draw();

	ui.Begin();
	{
		if (ui.BeginWindow("ofApp"))
		{
			ui.Add(colorQuantizer.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
			if (colorQuantizer.bGui) {
				ui.Indent();
				ui.Add(colorQuantizer.bGui_Picture, OFX_IM_TOGGLE_ROUNDED);
				ui.Add(colorQuantizer.bGui_Library, OFX_IM_TOGGLE_ROUNDED);
				ui.Unindent();
			}

			ui.AddToggle("Img", bDrawImg);
			ui.EndWindow();
		}

		colorQuantizer.drawImGuiWindows();
	}
	ui.End();

	//--

	// Image preview
	if (bDrawImg) {
		if (colorQuantizer.isUpdated())
		{
			img = colorQuantizer.getImage();
		}
		if (img.isAllocated())
		{
			ofRectangle r{ 0,0, img.getWidth(),img.getHeight() };
			r.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FILL);
			img.draw(r.x, r.y, r.getWidth(), r.getHeight());
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}
