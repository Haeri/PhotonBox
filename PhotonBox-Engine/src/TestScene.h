#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Resources/Scene.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "TransformerScript.cpp"
#include "Resources/OBJLoader.h"
#include "Components/PointLight.h"

class TestScene : public Scene {
public:
	Shader *shader;
	ForwardShader* forwardShader;
	Mesh *mesh, *mesh2;
	Material *material, *material2;
	Texture* tex;

	void Load() override;

	void OnUnload() override;
};
#endif // TEST_SCENE_H
