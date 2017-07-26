#ifndef RENDERER_H
#define RENDERER_H


#include <vector>
#include <iostream>
#include <algorithm>

#include "ObjectRenderer.h"

class Renderer {
public:
	bool isRunning;

	void init();
	void render();
	void destroy();
	void addToRenderQueue(ObjectRenderer *behaviour);
	void removeFromRenderQueue(ObjectRenderer *behaviour);
	void printList();
private:
	std::vector<ObjectRenderer*> renderQueue;
};

#endif /* defined(RENDERER_H) */