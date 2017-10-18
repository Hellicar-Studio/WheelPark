#pragma once

#include "ProbabilityDrawer.h"

class SineWave : public ProbabilityDrawer {
public:
	SineWave();
	void draw();
	void update();
	void loadShader(string shaderPath);
private:
	ofShader shader;
};