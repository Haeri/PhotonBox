#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "../Core/Component.h"
#include "../Core/Systems/Renderer.h"

class ObjectRenderer : public Component{
public:
	ObjectRenderer();
	virtual void init();
	virtual void render() = 0;
	virtual void onDestroy();
	void destroy();
};

#endif /* defined(OBJECT_RENDERER_H) */
