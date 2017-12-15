#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxAutoReloadedShader glitchShader;
		ofxAutoReloadedShader areaShader;
		ofFbo buffer;
		ofFbo imgBuffer;
		vector<ofImage> images;
		ofImage img, overlayImg, logoImg;
		ofImage probabilityFilter;

		ofxPanel gui;

		ofParameter<float> glitchSpeed;

		ofParameterGroup glitchGroup;
		ofParameter<float> glitchScale;
		ofParameter<ofVec4f> groupSize;
		ofParameter<ofVec4f> subGrid; 
		ofParameter<ofVec4f> blockSize;
		ofParameter<float> width;
		ofParameter<float> glitchMaskAmount;
		ofParameter<float> percentage;

		ofParameterGroup aberrationGroup;
		ofParameter<float> aberrationStrength;

		ofParameterGroup badTVGroup;
		ofParameter<float> badTVDistort;
		ofParameter<float> badTVDistort2;
		ofParameter<float> badTVSpeed;
		ofParameter<float> badTVRollSpeed;
		ofParameter<float> badTVAmount;

		bool showGui;
};
