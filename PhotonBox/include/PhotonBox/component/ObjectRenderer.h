#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

class LightEmitter;
class Material;
class Shader;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/data-type/BoundingSphere.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT ObjectRenderer : public Component
{
public:
	bool getReflected = true;
	bool castShadows = true;
	
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
