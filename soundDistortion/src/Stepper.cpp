#include "Stepper.h"

#define NUM_RECTANGLES 12

Stepper::Stepper() {
	//buffer.allocate(w, h);
}

int Stepper::getActiveRect(float x) {
	for (int i = 0; i < rectangles.size(); i++) {
		if (x > rectangles[i].position.x - rectangles[i].width / 2 && x < rectangles[i].position.x + rectangles[i].width / 2) {
			return i;
		}
	}
}

void Stepper::setup(int w, int h) {
	buffer.allocate(w, h);
	for (int i = 0; i < NUM_RECTANGLES; i++) {
		ofRectangle rect;
		rect.width = ofGetWidth() / NUM_RECTANGLES;
		rect.height = 0;
		rect.position.x = rect.width / 2 + i * rect.width;
		rect.position.y = ofGetHeight();
		rectangles.push_back(rect);
	}
}

void Stepper::update() {
	int rect = getActiveRect(ofGetMouseX());
	for (int i = 0; i < rectangles.size(); i++) {
		if (i == rect) {
			rectangles[i].height = ofLerp(rectangles[i].height, ofGetHeight() * 2, 0.1);
		}
		else {
			rectangles[i].height = ofLerp(rectangles[i].height, 0, 0.05);
		}
	}
}

void Stepper::draw() {
	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofSetColor(127, 0, 0, 255);
	for (int i = 0; i < rectangles.size(); i++) {
		ofDrawRectangle(rectangles[i]);
	}
	ofPopStyle();
}

void Stepper::drawToBuffer() {
	buffer.begin();
	ofSetColor(0);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	draw();
	buffer.end();
}

void Stepper::drawBuffer() {
	buffer.draw(0, 0);
}