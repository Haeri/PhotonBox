#include <algorithm>
#include <iostream>
#include "../header/Renderer.h"
#include "../header/Display.h"
#include "../header/ObjectRenderer.h"

bool Renderer::_isRunning;
std::vector<ObjectRenderer*> Renderer::_renderQueue;

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.push_back(objectRenderer);
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.erase(std::remove(_renderQueue.begin(), _renderQueue.end(), objectRenderer), _renderQueue.end());
}

void Renderer::init() {
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	Display::clearDisplay(0.1, 0.1, 0.1, 1);

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->render();
	}
	
	Display::swapBuffer();
}

void Renderer::destroy() {
	_renderQueue.clear();
}

void Renderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}