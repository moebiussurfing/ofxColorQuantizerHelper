#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxImGuiSurfing::SurfSetMyMonitor(0);

	ui.setup();

	//-

	// Reference first or clicked palette color
	// to be autoupdated. Will be used as Bg color.
	colorQuantizer.setColorPtr(c);

	colorQuantizer.setUiPtr(&ui);
	colorQuantizer.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(c);

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
			ofxImGuiSurfing::SurfDrawImageAtBottom(img);
		}
	}

	//--

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
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}
