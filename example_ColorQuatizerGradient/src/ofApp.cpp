#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::setMonitorsLayout(1, true, true);//right monitor portrait
	//ofxSurfingHelpers::setMonitorsLayout(-1, false, true);//left monitor landscape

	ui.setup();

	//--

	// Reference first or clicked palette color
	// to be auto updated. 
	// Will be used as Bg color.
	colorQuantizer.setColorPtr(c);

	colorQuantizer.setUiPtr(&ui);
	colorQuantizer.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	string s = "ofxColorQuantizerHelper | ";
	s += ofToString(ofGetFrameRate(), 1) + " FPS";
	ofSetWindowTitle(s);

	//--

	ofBackground(c);
	if (!colorQuantizer.isProcessing()) {
		int pad = 200;
		gradient.drawDebug(0, 0, ofGetWidth(), pad);
		ofSetColor(0, 200);
		ofSetLineWidth(2);
		ofDrawLine(0, pad, ofGetWidth(), pad);
	}

	//--

	// Image preview
	if (bDrawImg)
	{
		if (colorQuantizer.isUpdated()) // easy callback
		{
			// get image
			img = colorQuantizer.getImage();
		}

		if (colorQuantizer.isUpdatedbSorting()) // easy callback
		{
			// get colors
			gradient.reset();
			for (size_t i = 0; i < colorQuantizer.getPaletteSize(); i++) {
				gradient.addColor(colorQuantizer.getColor(i));
			}
		}

		// draw
		ofxSurfingHelpers::drawImageResponsive(img);
	}

	//--

	colorQuantizer.draw(); // Debug info. Not strictly required when not using OF native widgets!

	//--

	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui()
{
	ui.Begin();
	{
		if (ui.BeginWindow(bGui))
		{
			ui.Add(colorQuantizer.bGui, OFX_IM_TOGGLE_ROUNDED_MEDIUM);
			if (colorQuantizer.bGui) {
				ui.Indent();
				ui.Add(colorQuantizer.bGui_Picture, OFX_IM_TOGGLE_ROUNDED);
				ui.Add(colorQuantizer.bGui_Library, OFX_IM_TOGGLE_ROUNDED);
				ui.Unindent();
			}

			ui.AddSpacing();
			ui.AddToggle("Draw Image", bDrawImg, OFX_IM_TOGGLE_MEDIUM);

			ui.EndWindow();
		}

		colorQuantizer.drawImGuiWindows();
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'F') ofToggleFullscreen();
	if (key == 'G') bGui = !bGui;
}
