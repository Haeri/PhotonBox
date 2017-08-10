#include "../header/Lighting.h"

void Lighting::addLight(LightEmitter* light)
{
	lights.insert(std::pair<std::type_index, LightEmitter*>(typeid(light), light));
}

void Lighting::removeLight(LightEmitter* light)
{
//	lights.erase(lights.find(light));
}
