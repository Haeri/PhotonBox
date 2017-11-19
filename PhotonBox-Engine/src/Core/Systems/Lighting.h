#ifndef LIGHTING_H
#define LIGHTING_H

class LightEmitter;
class LightProbe;
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <algorithm>

class Lighting {
public:
	template<class T>
	static void addLight(T* light) {
		
		if (_lights.find(typeid(T)) != _lights.end()) {
			_lights[typeid(T)].push_back(light);
		}
		else {
			std::vector<LightEmitter*> vec;
			vec.push_back(light);
			_lights.insert(std::pair<std::type_index, std::vector<LightEmitter*>>(typeid(T), vec));
		}
	}

	template<class T>
	static void removeLight(T* light) {
		auto it = _lights.find(typeid(T));
		if (it != _lights.end()) {
			auto& vec = (_lights[typeid(T)]);
			vec.erase(std::remove(vec.begin(), vec.end(), light), vec.end());

			if (vec.size() == 0)
				_lights.erase(it);
		}
	}

	template<class T>
	static std::vector<T*>& getLights() {
		if (_lights.find(typeid(T)) == _lights.end()) {
			std::cerr << "Lights not found!" << std::endl;
//			return _lights.end();
		}
		return (std::vector<T*>&)_lights[typeid(T)];
	}

	static std::unordered_map<std::type_index, std::vector<LightEmitter*>>& getAllLights() {
		return _lights;
	}

	void start();

	static void addLightProbe(LightProbe* lightProbe) {
		_lightProbes.push_back(lightProbe);
	}

	static void removeFromLightProbeList(LightProbe* lightProbe) {
		_lightProbes.erase(std::remove(_lightProbes.begin(), _lightProbes.end(), lightProbe), _lightProbes.end());
	}
private:
	static std::unordered_map<std::type_index, std::vector<LightEmitter*>> _lights;
	static std::vector<LightProbe*> _lightProbes;
};

#endif // LIGHTING_H
