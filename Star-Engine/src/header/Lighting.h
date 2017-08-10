#ifndef LIGHTING_H
#define LIGHTING_H

#include <map>
#include <typeindex>
#include "LightEmitter.h"

class Lighting {
public:
	std::multimap<std::type_index, LightEmitter*> lights;

	Lighting() {}
	void addLight(LightEmitter* light);
	void removeLight(LightEmitter* light);
};

#endif /* defined(LIGHTING_H) */