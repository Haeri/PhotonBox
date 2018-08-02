#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class DepthShader;
class FrameBuffer;

#include "PhotonBox/components/LightEmitter.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/math/Matrix4f.h"

class DirectionalLight : public LightEmitter
{
public:
	Vector3f direction;
	Matrix4f lightProjection;
	FrameBuffer* shadowBuffer;

	DirectionalLight();
	~DirectionalLight();

	void OnEnable() override;
	void OnDisable() override;

	void destroy() override;
	Shader* getLightShader() override;
	void renderShadowMap(bool captureMode);
private:
	DepthShader* _depthShader;
	int _shadowMapResolution;
};

#endif // DIRECTIONAL_LIGHT_H
