#include "Lighting.h"

std::unordered_map<std::type_index, std::vector<LightEmitter*>> Lighting::_lights;