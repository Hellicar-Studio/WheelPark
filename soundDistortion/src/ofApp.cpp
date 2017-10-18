#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	glitchShader.load("Shaders/glitch");
	areaShader.load("shaders/area");

	img.load("ToyotaImage.png");
	probabilityFilter.load("probGradientLine.jpg");
	buffer.allocate(1920, 1080);

	string settingsPath = "settings/settings.xml";

	gui.setup("Glitch Parameters", settingsPath);
	glitchGroup.setName("Glitch");
	aberrationGroup.setName("Chromatic Aberration");
	badTVGroup.setName("Bad TV");

	gui.add(glitchSpeed.set("Speed", 1.0, 0.0, 5.0));

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

	stepper.setBuffer(&buffer);
	stepper.setup(1920, 1080);

	wave.setBuffer(&buffer);
	wave.loadShader("shaders/sineWave");

	//ofHideCursor();
}

//--------------------------------------------------------------
void ofApp::update(){
	stepper.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	wave.drawToBuffer();
	//stepper.drawToBuffer();

	//buffer.begin();
	////verticalBlur.begin();
	////verticalBlur.setUniform1f("blurAmnt", 500.0);
	////verticalBlur.setUniformTexture("diffuseTexture", stepper.buffer.getTexture(), 0);
	////ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//verticalBlur.end();
	//	areaShader.begin();
	//		areaShader.setUniform2f("u_mousePos", ofVec2f(ofGetMouseX(), ofGetMouseY()));
	//		ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
	//	areaShader.end();
	//buffer.end();

	//stepper.drawToBuffer();

	ofSetColor(255);
	glitchShader.begin();
		glitchShader.setUniformTexture("u_glitchMask", *(stepper.buffer), 1);
		glitchShader.setUniform1f("u_time", ofGetElapsedTimef() * glitchSpeed);
		glitchShader.setUniform1f("u_glitchScale", glitchScale);
		glitchShader.setUniform4f("u_groupSize", groupSize);
		glitchShader.setUniform4f("u_subGrid", subGrid);
		glitchShader.setUniform4f("u_blockSize", blockSize);
		glitchShader.setUniform1f("u_aberrationStrength", aberrationStrength);
		glitchShader.setUniform1f("u_badTVSpeed", badTVSpeed);
		glitchShader.setUniform1f("u_badTVAmount", badTVAmount);
		glitchShader.setUniform1f("u_badTVDistort", badTVDistort);
		glitchShader.setUniform1f("u_badTVDistort2", badTVDistort2);
		glitchShader.setUniform1f("u_badTVRollSpeed", badTVRollSpeed);
		img.draw(0, 0, 1920, 1080);
	glitchShader.end();

	//stepper.draw();

	if(showGui)
		gui.draw();
	//ofSetColor(255, 255, 255, 127);
	buffer.draw(0, 0);
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
