#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "../header/Scene.h"
#include "../header/MeshRenderer.h"

class TestScene : public Scene{
public:
	Mesh* mesh;
	Vertex* vertices;
	void Load() override {

		mesh = new Mesh();

		vertices = new Vertex[3]{
			Vertex(Vector3f(-0.5f, -0.5f, 0.0f)),
			Vertex(Vector3f(0.0f, 0.5f, 0.0f)),
			Vertex(Vector3f(0.5f, -0.5f, 0.0f)),
		};

		mesh->init(vertices, 3);

		GameObject* triangle = instanciate("Triangle");
		triangle->addComponent<MeshRenderer>()->setMesh(mesh);
	}

	void OnUnload() override {
		delete vertices;
		delete mesh;
	}
};

#endif /* defined(TEST_SCENE_H) */
