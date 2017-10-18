#pragma once

#include "ofMain.h"

class Stepper {
public:
	Stepper();
	void setup(int w, int h);
	void drawToBuffer();
	void drawBuffer();
	void draw();
	void update();
	ofFbo buffer;
private:
	vector<ofRectangle> rectangles;
	int getActiveRect(float x);
};