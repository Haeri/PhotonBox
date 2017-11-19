#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class ForwardPointLightShader;
#include "LightEmitter.h"

class PointLight : public LightEmitter {
public:
	float constant;
	float linear;
	float quadratic;
	
	PointLight();
	void destroy() override;
	Shader* getLightShader() { return (Shader*)_shader; }
private:
	ForwardPointLightShader* _shader;
};
#endif // POINT_LIGHT_H
