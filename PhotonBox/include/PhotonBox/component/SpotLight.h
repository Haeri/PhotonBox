#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "PhotonBox/component/LightEmitter.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT SpotLight : public LightEmitter
{
public:
	float constant = 2.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float coneAngle = 0.97f;
	float coneAttenuation = 0.96f;

	void OnEnable() override;
	void OnDisable() override;
	void init() override;
	void destroy() override;
	Shader* getLightShader() override;
};
#endif // SPOT_LIGHT_H
