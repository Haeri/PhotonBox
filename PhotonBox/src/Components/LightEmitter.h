#ifndef LIGHT_EMITTER_H
#define LIGHT_EMITTER_H

class Shader;
#include "../Core/Component.h"
#include "../Math/Vector3f.h"

class LightEmitter : public Component
{
public:
	Vector3f color;
	float intensity;

	virtual Shader* getLightShader() = 0;
};

#endif // LIGHT_EMITTER_H
