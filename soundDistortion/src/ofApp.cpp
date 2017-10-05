#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	shader.load("shaders/glitch");
	img.load("ToyotaImage.png");
	probabilityFilter.load("probGradientLine.jpg");
	buffer.allocate(1920, 1080);

	string settingsPath = "settings/settings.xml";

	gui.setup("Glitch Parameters", settingsPath);
	glitchGroup.setName("Glitch");
	aberrationGroup.setName("Chromatic Aberration");
	badTVGroup.setName("Bad TV");

	glitchGroup.add(glitchScale.set("Scale", 0.5, 0.0, 2.0));
	glitchGroup.add(groupSize.set("Group Size", ofVec4f(0.6, 0.8, 0.2, 1.0), ofVec4f(0.0), ofVec4f(1.0)));
	glitchGroup.add(subGrid.set("Subgrid", ofVec4f(2, 3, 6, 9), ofVec4f(1), ofVec4f(10)));
	glitchGroup.add(blockSize.set("Block Size", ofVec4f(0.2, 1, 1, 6), ofVec4f(0), ofVec4f(10)));
	glitchGroup.add(width.set("Width", 0.5, 0.0, 1.0));

	aberrationGroup.add(aberrationStrength.set("Aberration Strength", 100, 0.0, 1920));

	badTVGroup.add(badTVDistort.set("Distortion 1", 0, 0.0, 10));
	badTVGroup.add(badTVDistort2.set("Distortion 2", 0.5, 0.0, 1.0));
	badTVGroup.add(badTVSpeed.set("Speed", 0.5, 0.0, 1.0));
	badTVGroup.add(badTVRollSpeed.set("Roll Speed", 0.5, 0.0, 1.0));
	badTVGroup.add(badTVAmount.set("Amount", 0.5, 0.0, 1.0));

	gui.add(glitchGroup);
	gui.add(aberrationGroup);
	gui.add(badTVGroup);

	gui.loadFromFile(settingsPath);

	showGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushStyle();
	buffer.begin();
		ofSetColor(0);
		ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
		ofSetColor(255);
		ofPushMatrix();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofTranslate(ofGetMouseX(), ofGetHeight() / 2);
		ofScale(width, 1.0);
		ofRotate(90);
		probabilityFilter.draw(0, 0);
		ofPopMatrix();
	buffer.end();

	ofPopStyle();
	ofSetColor(255);
	shader.begin();
	shader.setUniform1f("u_time", ofGetElapsedTimef());
	shader.setUniformTexture("u_glitchMask", buffer, 1);
	shader.setUniform1f("u_glitchScale", glitchScale);
	shader.setUniform4f("u_groupSize", groupSize);
	shader.setUniform4f("u_subGrid", subGrid);
	shader.setUniform4f("u_blockSize", blockSize);
	shader.setUniform1f("u_aberrationStrength", aberrationStrength);
		img.draw(0, 0, 1920, 1080);
	shader.end();

	if(showGui)
		gui.draw();
	//ofSetColor(255, 255, 255, 127);
	//buffer.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'g') {
		showGui = !showGui;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
