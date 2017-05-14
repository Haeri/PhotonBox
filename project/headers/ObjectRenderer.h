#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "Component.h"

class ObjectRenderer : public Component{
public:
	ObjectRenderer();
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
	void setEnable(bool enable) { isEnabled = enable; }
	bool getEnable() { return isEnabled; }
private:
	bool isEnabled = true;
};

#endif /* defined(OBJECT_RENDERER_H) */