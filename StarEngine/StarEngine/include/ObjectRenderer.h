#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "Component.h"

class ObjectRenderer : public Component{
public:
	ObjectRenderer();
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void onDestroy();
	void destroy();
};

#endif /* defined(OBJECT_RENDERER_H) */