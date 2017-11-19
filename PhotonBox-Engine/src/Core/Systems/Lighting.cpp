#include "Lighting.h"
#include "../../Components/LightProbe.h"

std::unordered_map<std::type_index, std::vector<LightEmitter*>> Lighting::_lights;
std::vector<LightProbe*> Lighting::_lightProbes;

void Lighting::start(){
	for (std::vector<LightProbe*>::iterator it = _lightProbes.begin(); it != _lightProbes.end(); ++it) {
		(*it)->capture();
	}
}
