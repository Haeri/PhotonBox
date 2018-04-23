#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "../Core/Component.h"
#include "../Core/Systems/Renderer.h"
#include "../Resources/Material.h"
#include "../Core/BoundingSphere.h"

class ObjectRenderer : public Component
{
public:
	bool captureble = true;
	ObjectRenderer() : ObjectRenderer(RenderType::opaque) {}
	ObjectRenderer(RenderType type);
	virtual void init();
	virtual void render() = 0;
	virtual void render(Shader* shader) {}
	virtual void render(Shader* shader, LightEmitter* light) {}
	virtual void onDestroy();
	virtual BoundingSphere getBoundingSphere();
	void destroy();
	RenderType getType() { return _type; }
	void setRenderType(RenderType type);

	void setMaterial(Material* material) { _material = material; }
	Material* getMaterial() { return _material; }
protected:
	RenderType _type;
	Material* _material;
};

#endif /* defined(OBJECT_RENDERER_H) */
