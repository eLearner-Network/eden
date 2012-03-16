#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetWindowShape(640, 480);
    //ofSetDataPathRoot("data/");
    //cout << ofToDataPath("a") << endl;
}

//--------------------------------------------------------------
void testApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));	

}

//--------------------------------------------------------------
void testApp::draw() {
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
		case 'f':
			ofToggleFullscreen();
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

