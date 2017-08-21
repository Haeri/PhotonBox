#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "LightEmitter.h"
#include "../Core/Systems/Lighting.h"

class PointLight : public LightEmitter {
public:
	float constant;
	float linear;
	float quadratic;
	
	PointLight() {
		Lighting::addLight(this);
	}
};
#endif /* defined(POINT_LIGHT_H) */
