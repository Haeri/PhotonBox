#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "LightEmitter.h"
#include "../Core/Systems/Lighting.h"

class DirectionalLight : public LightEmitter {
public:
	Vector3f direction;

	DirectionalLight() {
		Lighting::addLight(this);
	}
};
#endif // DIRECTIONAL_LIGHT_H
