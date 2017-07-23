#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "../header/Scene.h"
#include "../header/MeshRenderer.h"

class TestScene : public Scene{
public:
	Shader *shader;
	Mesh *mesh, *mesh2;
	Material *material, *material2;
	Texture* tex;

	void Load() override {

		Matrix4f mat;
		std::cout << mat << std::endl;


		std::vector <Vertex> vertices = {
			Vertex(Vector3f(-0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(1, 0, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 0, 1), Vector2f(0, 0)),
		};
		std::vector<unsigned int>indices = {
			0, 1, 2,
		};

		std::vector <Vertex> vertices2 = {
			Vertex(Vector3f(-1.0f, -1.0f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(1, 0)),
			Vertex(Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 1)),
			Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(1, 1)),
		};
		std::vector<unsigned int>indices2 = {
			0, 1, 2, 1, 3, 2
		};

		mesh = new Mesh(vertices, indices);
		shader = new Shader("C:/Users/haeri/Documents/github/Space-Engine/Star-Engine/res/shader");
		material = new Material(shader);
		
		mesh2 = new Mesh(vertices2, indices2);
		tex = new Texture("C:/Users/haeri/Documents/github/Space-Engine/Star-Engine/res/debug.png");
		material2 = new Material(shader, tex);

		GameObject* triangle = instanciate("Triangle");
		triangle->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		triangle->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		triangle->getComponent<Transform>()->setScale(Vector3f(1, 1, 1));
		triangle->addComponent<MeshRenderer>();
		triangle->getComponent<MeshRenderer>()->setMesh(mesh);
		triangle->getComponent<MeshRenderer>()->setMaterial(material);

		GameObject* quad = instanciate("Quad");
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(mesh2);
		quad->getComponent<MeshRenderer>()->setMaterial(material2);
	}

	void OnUnload() override {
		delete shader;
		delete mesh;
		delete material;
	}
};

#endif /* defined(TEST_SCENE_H) */
