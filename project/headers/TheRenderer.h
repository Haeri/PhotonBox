#ifndef THE_RENDERER_H
#define THE_RENDERER_H

class ObjectRenderer;
class Renderer;
#include <vector>

class TheRenderer {
public:
	Renderer* renderer;
	bool* isRunning;

	TheRenderer(){
		if (instance == nullptr) {
			instance = this;
		}
	}
	
	void setRenderer(Renderer* rend, bool*_isRunning) {
		isRunning = _isRunning;
		renderer = rend;
	}
	void init();
	void render();
	void destroy();
	static TheRenderer* Instance();
	static void addToRenderQueue(ObjectRenderer *behaviour);
	static void printList();
private:
	static TheRenderer* instance;
	static std::vector<ObjectRenderer*> renderQueue;
};

#endif /* defined(THE_RENDERER_H) */