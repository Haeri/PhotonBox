#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class DepthShader;
#include "../Core/OpenGL.h"
#include "../Math/Matrix4f.h"
#include "LightEmitter.h"

class DirectionalLight : public LightEmitter
{
public:
	Vector3f direction;
	Matrix4f lightProjection;
	FrameBuffer* shadowBuffer;

	DirectionalLight();
	void destroy() override;
	Shader* getLightShader() override;
	void renderShadowMap(bool captureMode);
private:
	DepthShader* _depthShader;
	GLuint _shadowMapResolution;
};
#endif // DIRECTIONAL_LIGHT_H