#pragma once

#include "ProbabilityDrawer.h"
#include "ofMain.h"

class Stepper : public ProbabilityDrawer {
public:
	Stepper();
	void setup(int w, int h);
	void draw();
	void update();
private:
	vector<ofRectangle> rectangles;
	int getActiveRect(float x);
};