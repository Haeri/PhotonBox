#include "Lighting.h"

std::unordered_map<std::type_index, std::vector<LightEmitter*>> Lighting::_lights;

/*
void Lighting::addLight(LightEmitter* light)
{
	if (_lights.find(typeid(light)) != _lights.end()) {
		_lights[typeid(light)].push_back(light);
	}else {
		std::vector<LightEmitter*> vec;
		vec.push_back(light);
		_lights.insert(std::pair<std::type_index, std::vector<LightEmitter*>>(typeid(light), vec));
	}
}
*/

/*void Lighting::removeLight(LightEmitter* light)
{
	// TODO: Make this happen
	
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

}
*/