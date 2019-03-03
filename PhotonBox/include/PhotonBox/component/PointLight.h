#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "PhotonBox/component/LightEmitter.h"

class PointLight : public LightEmitter
{
public:
	float constant = 2;
	float linear = 0.09f;
	float quadratic = 0.032f;

	PointLight();

	void OnEnable() override;
	void OnDisable() override;
	void destroy() override;
	Shader* getLightShader() override;
};

#endif // POINT_LIGHT_H
