#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class ForwardDirectionalLightShader;
#include "LightEmitter.h"

class DirectionalLight : public LightEmitter {
public:
	Vector3f direction;

	DirectionalLight();
	void destroy() override;
	Shader* getLightShader() override { return (Shader*)_shader; }
private: 
	ForwardDirectionalLightShader* _shader;
};
#endif // DIRECTIONAL_LIGHT_H
