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

	ofBackground(c);

	//--

	ofPushMatrix();

	// make space for top color picked bar
	int h_spc = 30;
	ofTranslate(0, h_spc);

	int h_gradient = 225;
	if (!colorQuantizer.isProcessing())
	{
		ofPushStyle();
		gradient.drawDebug(0, 0, ofGetWidth(), h_gradient);
		ofSetColor(0, 200);
		ofSetLineWidth(2);
		ofDrawLine(0, h_gradient, ofGetWidth(), h_gradient);
		ofPopStyle();
	}

	//--

	// Image preview
	if (bDrawImg)
	{
		// update
		if (colorQuantizer.isUpdatedbSorting()) // easy callback
		{
			// get colors
			gradient.reset();
			for (size_t i = 0; i < colorQuantizer.getPaletteSize(); i++) {
				gradient.addColor(colorQuantizer.getColor(i));
			}
		}

		// get image
		if (colorQuantizer.isUpdatedImage()) // easy callback
		{
			// get image
			img = colorQuantizer.getImage();
		}

		//--

		// draw
		if (img.isAllocated())
		{
			//ofTranslate(0, h_spc/2);

			if (ofGetWidth() > ofGetHeight()) {
				ofxSurfingHelpers::drawImageAtRight(img);//landscape
			}
			else {
				ofPushStyle();

				ofRectangle r{ 0,0, img.getWidth(),img.getHeight() };
				r.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);
				r.translateY(ofGetHeight() - r.getBottomLeft().y);

				float h = r.getHeight();
				float ydiff = ofGetHeight() - (h + h_gradient);
				ofTranslate(0, -ydiff);

				ofSetColor(255, 255);
				img.draw(r.x, r.y, r.getWidth(), r.getHeight());

				//line
				ofSetColor(0, 200);
				ofSetLineWidth(2);
				ofDrawLine(r.getBottomLeft(), r.getBottomRight());
				//ofDrawLine(r.getTopLeft(), r.getTopRight());

				ofPopStyle();
			}
		}
	}

	ofPopMatrix();

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

			ui.AddSpacingBigSeparated();
			ui.DrawWidgetsGlobalScaleMini();

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
