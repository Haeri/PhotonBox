#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "../Core/Component.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/Material.h"

class ObjectRenderer : public Component{
public:
	ObjectRenderer();
	virtual void init();
	virtual void render() = 0;
	virtual void render(Shader* shader) {}
	virtual void render(Shader* shader, LightEmitter* light) {}
	virtual void onDestroy();
	void destroy();

	void setMaterial(Material* material) { _material = material; }
	Material* getMaterial() { return _material; }
protected:
	Material* _material;
};

#endif /* defined(OBJECT_RENDERER_H) */