#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "LightEmitter.h"
#include "../Core/Systems/Lighting.h"

class AmbientLight : public LightEmitter {
public:
	AmbientLight() {
		Lighting::addLight(this);
	}
};
#endif // AMBIENT_LIGHT_H
