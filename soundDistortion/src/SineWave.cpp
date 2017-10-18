#include "SineWave.h"

SineWave::SineWave() {
	ProbabilityDrawer();
	setupGui();
}

void SineWave::draw() {
	shader.begin();
		shader.setUniform2f("u_mousePos", ofVec2f(ofGetMouseX(), ofGetMouseY()));
		shader.setUniform1f("u_amp", amplitude);
		shader.setUniform1f("u_blur",blur);
		ofDrawRectangle(0, 0, buffer->getWidth(), buffer->getHeight());
	shader.end();
}

void SineWave::update() {

}

void SineWave::loadShader(string shaderPath) {
	shader.load(shaderPath);
}

void SineWave::setupGui() {
	ProbabilityDrawer::setupGui();
	gui.add(amplitude.set("amp", 0.3, 0.0, 2.0));
	gui.add(blur.set("blur", 0.0, 0.0, 2.0));
}