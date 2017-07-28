#include "../header/Renderer.h"
#include "../header/Core.h"


void Renderer::addToRenderQueue(ObjectRenderer *objectRenderer) {
	renderQueue.push_back(objectRenderer);
}

void Renderer::removeFromRenderQueue(ObjectRenderer *objectRenderer) {
	renderQueue.erase(std::remove(renderQueue.begin(), renderQueue.end(), objectRenderer), renderQueue.end());
}

void Renderer::init() {
	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		(*it)->init();
	}
}

void Renderer::render() {
	
	Core::getInstance()->display.clearDisplay(0.1, 0.1, 0.1, 1);

	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		if ((*it)->getEnable())
			(*it)->render();
	}
	
	Core::getInstance()->display.swapBuffer();
}

void Renderer::destroy() {
	renderQueue.clear();
}

void Renderer::printList(){
	for (std::vector<ObjectRenderer*>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it) {
		std::cout << (*it)->getName() << std::endl;
	}
}