#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "PhotonBox/core/Component.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/resources/Material.h"
#include "PhotonBox/core/BoundingSphere.h"

class ObjectRenderer : public Component
{
public:
	bool getReflected = true;
	bool castShadows = true;

	ObjectRenderer() : ObjectRenderer(RenderType::opaque) {}
	ObjectRenderer(RenderType type);
	virtual ~ObjectRenderer() {}

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

#endif // OBJECT_RENDERER_H
