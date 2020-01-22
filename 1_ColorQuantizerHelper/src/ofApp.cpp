#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);

	colorQuantizer.setup();
    colorQuantizer.setPosition(glm::vec2(200, 200));
    colorQuantizer.setSize(glm::vec2(1000, 700));
}

//--------------------------------------------------------------
void ofApp::update(){
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

	if (key == ' ')
	{
		colorQuantizer.setActive(!colorQuantizer.isActivated());
	}
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
