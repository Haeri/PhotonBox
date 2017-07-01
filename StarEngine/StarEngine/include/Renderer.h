#ifndef RENDERER_H
#define RENDERER_H

class ObjectRenderer;
class Renderer;
#include <vector>

class Renderer {
public:
	static bool isRunning;

	void init();
	void render();
	void destroy();
	static void addToRenderQueue(ObjectRenderer *behaviour);
	static void removeFromRenderQueue(ObjectRenderer *behaviour);
	static void printList();
private:
	static std::vector<ObjectRenderer*> renderQueue;
};

#endif /* defined(RENDERER_H) */