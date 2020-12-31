#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	colorQuantizer.setup();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(100,100,100);

	colorQuantizer.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'G') colorQuantizer.setToggleActive();
}
