#include "SineWave.h"

SineWave::SineWave() {
	ProbabilityDrawer();
}

void SineWave::draw() {
	shader.begin();
	ofDrawRectangle(0, 0, buffer->getWidth(), buffer->getHeight());
	shader.end();
}

void SineWave::update() {

}

void SineWave::loadShader(string shaderPath) {
	shader.load(shaderPath);
}