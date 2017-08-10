#ifndef LIGHT_EMITTER_H
#define LIGHT_EMITTER_H

#include "Component.h"
#include "Vector3f.h"

class LightEmitter : public Component {
public:
	Vector3f color;
	float intensity;

	LightEmitter() {
		//Core::getInstance()->lighting.addLight(this);
	}

	LightEmitter(Vector3f color, float intensity) : color(color), intensity(intensity) {
		//Core::getInstance()->lighting.addLight(this);
	}
};

#endif /* defined(LIGHT_EMITTER_H) */