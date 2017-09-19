#include <algorithm>
#include <iostream>
#include "Renderer.h"
#include "../Display.h"
#include "../../Components/ObjectRenderer.h"
#include "../../Components/Transform.h"
#include "../../Resources/Skybox.h"

bool Renderer::_isDebug;
SkyBox Renderer::_skyBox;
std::vector<ObjectRenderer*> Renderer::_renderQueue;
Renderer::RenderMode Renderer::renderMode;

void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.push_back(objectRenderer);
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	_renderQueue.erase(std::remove(_renderQueue.begin(), _renderQueue.end(), objectRenderer), _renderQueue.end());
}

void Renderer::setSkyBox(CubeMap* cubeMap){
	_skyBox.setCubeMap(cubeMap);
}

void Renderer::init(RenderMode mode) {
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	renderMode = mode;
	_isDebug = false;
}

void Renderer::start() {
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	Display::clearDisplay(0.1, 0.1, 0.1, 1);

	_skyBox.render();

	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		if ((*it)->getEnable()) {
			(*it)->render();
		}
	}
	
	if(_isDebug){
		for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
			if ((*it)->getEnable()) {
				(*it)->transform->renderHandels();
			}
		}
	}

	Display::swapBuffer();
}

void Renderer::setDebug(bool debug) {
	_isDebug = debug;
}

void Renderer::destroy() {
	_renderQueue.clear();
}

void Renderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = _renderQueue.begin(); it != _renderQueue.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}
