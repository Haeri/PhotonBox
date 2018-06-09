#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "PhotonBox/components/LightEmitter.h"

class PointLight : public LightEmitter
{
public:
	float constant;
	float linear;
	float quadratic;

	PointLight();

	void destroy() override;
	Shader* getLightShader() override;
};

#endif // POINT_LIGHT_H
