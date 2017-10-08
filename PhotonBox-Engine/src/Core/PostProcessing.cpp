#include "PostProcessing.h"
#include "../Resources/PostShader.h"

void PostProcessing::init() {
	Shader* schader = new PostShader("./res/post-processing/monochrom");
	normal = new Material(schader);
	mono = new FrameBuffer(Display::getWidth(), Display::getHeight());
}

void PostProcessing::preProcess() {
	mono->enable();
}

void PostProcessing::postProcess() {
	FrameBuffer::resetDefaultBuffer();
	glDisable(GL_DEPTH_TEST);
	mono->render(normal);
}

void PostProcessing::destroy() {
	mono->destroy();
}