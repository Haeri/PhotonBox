#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class ForwardDirectionalLightShader;
class DepthShader;
#include "../Math/Matrix4f.h"
#include "LightEmitter.h"
#include "../Core/Display.h"

class DirectionalLight : public LightEmitter {
public:
	Vector3f direction;
	Matrix4f lightProjection;
	GLuint _depthMap;

	DirectionalLight();
	void destroy() override;
	Shader* getLightShader() override { return (Shader*)_shader; }
	void renderShadowMap();
private: 
	ForwardDirectionalLightShader* _shader;
	DepthShader* _depthShader;

	GLuint _shadowMapResolution;
	GLuint _depthMapFBO;
};
#endif // DIRECTIONAL_LIGHT_H
