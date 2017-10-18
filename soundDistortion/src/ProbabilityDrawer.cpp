#include "ProbabilityDrawer.h"

ProbabilityDrawer::ProbabilityDrawer() {

}

void ProbabilityDrawer::drawToBuffer() {
	buffer->begin();
	draw();
	buffer->end();

}

void ProbabilityDrawer::update() {

}

void ProbabilityDrawer::draw() {

}

void ProbabilityDrawer::clearBuffer() {
	buffer->begin();
	buffer->clear();
	buffer->end();
}

void ProbabilityDrawer::setupGui() {
	gui.setup();
	gui.setPosition(100, 100);
}