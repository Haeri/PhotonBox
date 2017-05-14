#ifndef B_MATERIAL_DATA_H
#define B_MATERIAL_DATA_H

#include <unordered_map>
#include "vmmlib/vector.hpp"

struct MaterialData
{
    typedef std::unordered_map<std::string, std::string>		TextureMap;
	typedef std::unordered_map<std::string, std::string>		CubeRelflectionMap;
	typedef std::unordered_map<std::string, vmml::Vector3f>		Vector3Map;
	typedef std::unordered_map<std::string, GLfloat>			ScalarMap;
    
	TextureMap	textures;
	Vector3Map	vectors;
	ScalarMap	scalars;
	CubeRelflectionMap cubeTextures;
    
    std::string name;
};

#endif /* defined(B_MATERIAL_DATA_H) */
