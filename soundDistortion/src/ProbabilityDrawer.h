#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ProbabilityDrawer {
public:
	ProbabilityDrawer();
	ofFbo* buffer;
	void drawToBuffer();
	virtual void draw();
	virtual void update();
	void clearBuffer();
	void setBuffer(ofFbo* _buffer) { buffer = _buffer; };
	void drawGui() { 
		gui.draw(); 
	};
	void setupGui();
	ofxPanel gui;
private:
};