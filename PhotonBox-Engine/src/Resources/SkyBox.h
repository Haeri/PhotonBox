#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "CubeMap.h"
#include "SkyBoxShader.h"
#include "Mesh.h"

class SkyBox {
public:
	void setEnviromentMap(CubeMap* cubeMap);
	CubeMap* getEnviromentMap() { return _evMap; }
	void setIrradienceMap(CubeMap* cubeMap);
	CubeMap* getIrradienceMap() { return _irrMap; }

	void init();
	void render();
private:
	CubeMap* _evMap;
	CubeMap* _irrMap;
	SkyBoxShader* _skyBoxShader;
	GLuint _vao, _vbo, _ebo;
	Mesh* _mesh;

	void genVAO();	
};

#endif // SKY_BOX_H