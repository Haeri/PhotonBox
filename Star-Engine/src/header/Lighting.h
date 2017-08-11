#ifndef LIGHTING_H
#define LIGHTING_H

class LightEmitter;
#include <map>
#include <typeindex>

class Lighting {
public:
	static void addLight(LightEmitter* light);
	static void removeLight(LightEmitter* light);
private:
	static std::multimap<std::type_index, LightEmitter*> _lights;
};

#endif // LIGHTING_H