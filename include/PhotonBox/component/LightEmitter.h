#ifndef LIGHT_EMITTER_H
#define LIGHT_EMITTER_H

class Shader;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/math/Vector3f.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT LightEmitter : public Component
{
public:
	Vector3f color = Vector3f::ONE;
	float intensity = 1;

	virtual ~LightEmitter() {}

	virtual Shader* getLightShader() = 0;
	std::type_index getBaseType() override { return typeid(LightEmitter); }
};

#endif // LIGHT_EMITTER_H
