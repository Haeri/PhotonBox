#include "LightEmitter.h"
#include "../Core/Systems/Lighting.h"

LightEmitter::LightEmitter() {
	Lighting::addLight(this);
}

LightEmitter::LightEmitter(Vector3f color, float intensity) : color(color), intensity(intensity) {
	Lighting::addLight(this);
}
