#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "PhotonBox/components/LightProbe.h"
#include "PhotonBox/resources/CubeMap.h"
#include "PhotonBox/resources/Mesh.h"
#include "PhotonBox/resources/SkyBoxShader.h"

class SkyBox
{
public:
	float intensity = 1;
	void setLightProbe(LightProbe* lightProbe);

	LightProbe* getLightProbe() { return _lp; }
	void init();
	void render();
private:
	LightProbe * _lp;

	SkyBoxShader* _skyBoxShader;
	GLuint _vao, _vbo, _ebo;
	Mesh* _mesh;

	void genVAO();
};

#endif // SKY_BOX_H
