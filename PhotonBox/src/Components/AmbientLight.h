#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "LightEmitter.h"

class AmbientLight : public LightEmitter
{
public:
	AmbientLight();
	void destroy() override;
	Shader* getLightShader() override;
};
#endif // AMBIENT_LIGHT_H
