#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	glitchShader.load("Shaders/glitch");
	areaShader.load("shaders/area");

	img.load("cokeSantaNoLogo.jpg");
	probabilityFilter.load("probGradientLine.jpg");
	buffer.allocate(img.getWidth(), img.getHeight());

	string settingsPath = "settings/settings.xml";

	gui.setup("Glitch Parameters", settingsPath);
	glitchGroup.setName("Glitch");
	aberrationGroup.setName("Chromatic Aberration");
	badTVGroup.setName("Bad TV");

	gui.add(glitchSpeed.set("Speed", 1.0, 0.0, 5.0));
	gui.add(glitchMaskAmount.set("Glitch Mask Amount", 0, 0, 255));
	gui.add(percentage.set("Percentage", 0, 0, 100));

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

	ofHideCursor();

	overlayImg.load("ProbGradientVert.png");
	logoImg.load("HappyHolidays/happyhappy/cokeSanta-Logo.png");
	images.resize(6);
	for (int i = 2; i < 8; i++) {
		images[i-2].load("HappyHolidays/happyhappy/cokeSanta-" + ofToString(i) + ".png");
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	//glitchMaskAmount = ofMap(percentage, 0, 100, 0, 255);
}

//--------------------------------------------------------------
void ofApp::draw(){
	buffer.begin();
		ofPushStyle();
			ofClear(0);
			ofSetColor(0, 0, 0, 255);
			ofDrawRectangle(0, 0, ofGetWidth(), buffer.getHeight());
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofSetColor(glitchMaskAmount);
			overlayImg.draw(buffer.getWidth()/2, buffer.getHeight()/2, img.getWidth(), img.getHeight());
			float width = logoImg.getWidth();
			float ratio = img.getWidth() / width;
			float height = logoImg.getHeight() * ratio;
			logoImg.draw(buffer.getWidth() / 2, buffer.getHeight() / 2, img.getWidth(), height);
			ofPopStyle();
		//areaShader.begin();
		//	areaShader.setUniform2f("u_mousePos", ofVec2f(ofGetMouseX(), ofGetMouseY()));
		//	ofDrawRectangle(0, 0, buffer.getWidth(), buffer.getHeight());
		//areaShader.end();
	buffer.end();

	ofPushMatrix();
	ofDrawRectangle(0, 0, img.getWidth() + 200, img.getHeight() + 200);
	ofTranslate(100, 100);
	ofSetColor(255);
	glitchShader.begin();
		glitchShader.setUniformTexture("u_glitchMask", buffer, 1);
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
		img.draw(0, 0);
	glitchShader.end();
	ofPushStyle();
		//ofSetRectMode(OF_RECTMODE_CENTER);
		for (int i = 0; i < images.size(); i++) {
			float step = percentage.getMax() / images.size();
			float alpha = ofMap(percentage, i*step, ((i + 1) * step), 0, 255, true);
			ofSetColor(255, 255, 255, alpha);
			images[i].draw(0, 0);
		}
		//width = messageImg.getWidth();
		//ratio = img.getWidth() / width;
		//height = messageImg.getHeight() * ratio;
		//messageImg.draw(img.getWidth() / 2, img.getHeight() - height / 2, img.getWidth(), height);
	ofPopStyle();
	ofPopMatrix();

	//buffer.draw(0, 0);

	if(showGui)
		gui.draw();
	//ofSetColor(255, 255, 255, 127);
	//buffer.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'g') {
		showGui = !showGui;
		if (showGui)
			ofShowCursor();
		else
			ofHideCursor();
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
