#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "LightEmitter.h"

class SpotLight : public LightEmitter
{
public:
	float constant;
	float linear;
	float quadratic;
	float coneAngle;
	float coneAttenuation;

	SpotLight();
	void destroy() override;
	Shader* getLightShader() override;
};
#endif // SPOT_LIGHT_H