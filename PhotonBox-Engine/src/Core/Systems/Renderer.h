#ifndef RENDERER_H
#define RENDERER_H

class ObjectRenderer;
#include <vector>

class Renderer {
public:
	enum RenderMode
	{
		CUSTOM,
		FORWARD,
		DEFERRED
	};
	static RenderMode renderMode;
	static bool isDebug() { return _isDebug; }
	static void setDebug(bool debug);
	static void addToRenderQueue(ObjectRenderer *behaviour);
	static void removeFromRenderQueue(ObjectRenderer *behaviour);
	static void printList();

	void init(RenderMode);
	void start();
	void render();
	void destroy();
private:
	static bool _isDebug;
	static std::vector<ObjectRenderer*> _renderQueue;
};

#endif // RENDERER_H
