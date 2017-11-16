#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "CubeMap.h"
#include "SkyBoxShader.h"
#include "Mesh.h"
#include "../Components/LightProbe.h"

class SkyBox {
public:
	void setLightProbe(LightProbe* lightProbe);

	LightProbe* getLightProbe() { return _lp; }
	void init();
	void render();
private:
	LightProbe* _lp;
	
	SkyBoxShader* _skyBoxShader;
	GLuint _vao, _vbo, _ebo;
	Mesh* _mesh;

	void genVAO();	
};

#endif // SKY_BOX_H