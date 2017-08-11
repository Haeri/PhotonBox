#include "../header/Lighting.h"

std::multimap<std::type_index, LightEmitter*> Lighting::_lights;

void Lighting::addLight(LightEmitter* light)
{
	_lights.insert(std::pair<std::type_index, LightEmitter*>(typeid(light), light));
}

void Lighting::removeLight(LightEmitter* light)
{
	// TODO: Make this happen
	/*
	_lights.erase(_lights.find(light));


	typedef multimap<std::type_index, LightEmitter*>::iterator iterator;
	std::pair<iterator, iterator> iterpair = _lights.equal_range(light);

	iterator it = iterpair.first;
	for (; it != iterpair.second; ++it) {
		if (it->second == 15) {
			_lights.erase(it);
			break;
		}
	}
	*/
}
