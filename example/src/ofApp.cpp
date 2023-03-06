#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);

	ofSetWindowPosition(-1080, 25);
	ofSetWindowShape(1080, 1920);

	ui.setup();

	//-

	// Reference first or clicked palette color
	// to be autoupdated. Will be used as Bg color.
	colorQuantizer.setColor_BACK(c);

	colorQuantizer.setUiPtr(&ui);
	colorQuantizer.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(c);

	colorQuantizer.draw(); // not required when not using OF native widgets!

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
	if (bDrawImg)
	{
		if (colorQuantizer.isUpdated())
		{
			img = colorQuantizer.getImage();
		}
		if (img.isAllocated())
		{
			ofRectangle r{ 0,0, img.getWidth(),img.getHeight() };
			r.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);
			//r.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FILL);
			img.draw(r.x, r.y, r.getWidth(), r.getHeight());
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}
