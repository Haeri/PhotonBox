#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "CubeMap.h"
#include "SkyBoxShader.h"
#include "Mesh.h"

class SkyBox {
public:
	void setCubeMapSpecular(CubeMap* cubeMap);
	void setCubeMapDefuse(CubeMap* cubeMap);
	CubeMap* getCubeMapSpecular() { return _cubeMapSpecular; }
	CubeMap* getCubeMapDefuse() { return _cubeMapDefuse; }
	void init();
	void render();
private:
	CubeMap* _cubeMapSpecular, *_cubeMapDefuse;
	SkyBoxShader* _skyBoxShader;
	GLuint _vao, _vbo, _ebo;
	Mesh* _mesh;

	void genVAO();	
};

#endif // SKY_BOX_H