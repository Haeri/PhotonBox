#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "../Resources/ForwardDirectionalLightShader.h"
#include "LightEmitter.h"
#include "../Core/Systems/Lighting.h"

class DirectionalLight : public LightEmitter {
public:
	Vector3f direction;

	DirectionalLight() {
		Lighting::addLight(this);
		_shader = ForwardDirectionalLightShader::getInstance();
	}

	void destroy() override {
		Lighting::removeLight(this);
	}

	Shader* getLightShader() override { return _shader; }

private: 

	ForwardDirectionalLightShader* _shader;
};
#endif // DIRECTIONAL_LIGHT_H
