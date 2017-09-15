#ifndef PBR_SCENE_H
#define PBR_SCENE_H

class BasicShader;
class LitShader;

#include "Resources/Scene.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "TransformerScript.cpp"
#include "Resources/OBJLoader.h"
#include "Components/PointLight.h"

class PBRScene : public Scene {
public:
	BasicShader* basicShader;
	ForwardShader* forwardShader_;
	Mesh *plane, *sphere;
	Material *material, *material2;
	Texture* tex;

	void Load() override;

	void OnUnload() override;
};
#endif // PBR_SCENE_H
