#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);

	colorQuantizer.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	colorQuantizer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(100,100,100);

	colorQuantizer.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	colorQuantizer.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
