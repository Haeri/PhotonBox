#include <iostream>
#include "TheRenderer.h"
#include "ObjectRenderer.h"

TheRenderer* TheRenderer::instance;
std::vector<ObjectRenderer*> TheRenderer::renderQueue;


void TheRenderer::addToRenderQueue(ObjectRenderer *objectRenderer) {
	renderQueue.push_back(objectRenderer);
}

void TheRenderer::init() {
	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		(*it)->init();
	}
}

void TheRenderer::render() {
	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->render();
	}
}

void TheRenderer::destroy() {
	// TODO: remove from list
	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		(*it)->destroy();
	}
}

TheRenderer* TheRenderer::Instance() {
	return instance;
}

void TheRenderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}