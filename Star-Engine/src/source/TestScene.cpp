#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "../header/Scene.h"
#include "../header/MeshRenderer.h"

class TestScene : public Scene{
public:
	Mesh* mesh;
	Material* material;

	void Load() override {

		std::vector <Vertex> vertices = {
			Vertex(Vector3f(-0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(1, 0, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 0, 1), Vector2f(0, 0)),
		};
		std::vector<unsigned int>indices = {
			0, 1, 2
		};

		mesh = new Mesh(vertices, indices);
		material = new Material(Shader("C:/Users/haeri/Documents/github/Space-Engine/Star-Engine/res/shader"));
	
		GameObject* triangle = instanciate("Triangle");
		triangle->addComponent<MeshRenderer>();
		triangle->getComponent<MeshRenderer>()->setMesh(mesh);
		triangle->getComponent<MeshRenderer>()->setMaterial(material);
	}

	void OnUnload() override {
		delete mesh;
		delete material;
	}
};

#endif /* defined(TEST_SCENE_H) */
