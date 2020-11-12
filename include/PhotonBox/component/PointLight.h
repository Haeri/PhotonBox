#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "PhotonBox/component/LightEmitter.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT PointLight : public LightEmitter
{
public:
	float constant = 2;
	float linear = 0.09f;
	float quadratic = 0.032f;

	void OnEnable() override;
	void OnDisable() override;
	void init() override;
	void destroy() override;
	Shader* getLightShader() override;
};

#endif // POINT_LIGHT_H
