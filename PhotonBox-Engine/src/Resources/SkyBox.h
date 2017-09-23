#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "CubeMap.h"
#include "SkyBoxShader.h"
#include "Mesh.h"

class SkyBox {
public:
	void setCubeMap(CubeMap* cubeMap);
	CubeMap* getCubeMap() { return _cubeMap; }
	void init();
	void render();
private:
	CubeMap* _cubeMap;
	SkyBoxShader* _skyBoxShader;
	GLuint _vao, _vbo, _ebo;
	Mesh* _mesh;

	void genVAO();	
};

#endif // SKY_BOX_H