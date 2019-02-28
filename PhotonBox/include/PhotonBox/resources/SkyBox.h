#ifndef SKY_BOX_H
#define SKY_BOX_H

class SkyBoxShader;
class LightMap;

#include "PhotonBox/math/Vector3f.h"
#include "PhotonBox/resources/CubeMap.h"

class SkyBox
{
public:
	enum SkyBoxMode
	{
		DRAW_CUBE_MAP,
		DRAW_SOLID_COLOR
	};

	float intensity = 1;

	SkyBox() {}
	~SkyBox();

	void setCubeMap(CubeMap* cubeMap);
	void setColor(Vector3f color);
	void setDrawMode(SkyBoxMode mode);
	LightMap* getLightMap();

	void init();
	void render();
	void reset();
private:
	SkyBoxMode _mode;
	Vector3f _color;
	LightMap* _lightMap = nullptr;
	SkyBoxShader* _skyBoxShader;
	static Mesh* _mesh;
};

#endif // SKY_BOX_H
