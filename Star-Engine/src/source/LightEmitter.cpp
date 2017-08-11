#include "../header/LightEmitter.h"
#include "../header/Lighting.h"

LightEmitter::LightEmitter() {
	Lighting::addLight(this);
}

LightEmitter::LightEmitter(Vector3f color, float intensity) : color(color), intensity(intensity) {
	Lighting::addLight(this);
}