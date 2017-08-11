#ifndef LIGHT_EMITTER_H
#define LIGHT_EMITTER_H

#include "Component.h"
#include "Vector3f.h"

class LightEmitter : public Component {
public:
	Vector3f color;
	float intensity;

	LightEmitter();
	LightEmitter(Vector3f, float);
};

#endif // LIGHT_EMITTER_H