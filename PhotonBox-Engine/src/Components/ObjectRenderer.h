#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "../Core/Component.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/Material.h"

class ObjectRenderer : public Component{
public:
	ObjectRenderer() : ObjectRenderer(true) {}
	ObjectRenderer(bool isOpaque);
	virtual void init();
	virtual void render() = 0;
	virtual void render(Shader* shader) {}
	virtual void render(Shader* shader, LightEmitter* light) {}
	virtual void onDestroy();
	virtual float getBoundingSphereRadius() = 0;
	void destroy();
	bool isOpaque() { return _isOpaque; }

	void setMaterial(Material* material) { _material = material; }
	Material* getMaterial() { return _material; }
protected:
	bool _isOpaque;
	Material* _material;
};

#endif /* defined(OBJECT_RENDERER_H) */