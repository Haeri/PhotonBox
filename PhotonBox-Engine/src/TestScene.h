#ifndef TEST_SCENE_H
#define TEST_SCENE_H

class BasicShader;
class LitShader;

#include "Resources/Scene.h"
#include "Components/Camera.h"
#include "TransformerScript.cpp"
#include "Resources/OBJLoader.h"
#include "Components/PointLight.h"
#include "Resources/BasicShader.h"
#include "Resources/ForwardShader.h"
#include "Resources/LitShader.h"
#include "PrinterScript.cpp"
#include "Components/MeshRenderer.h"
#include "Components/PointRenderer.h"
#include "Components/AxisRenderer.h"
#include "CameraController.h"
#include "Components/AmbientLight.h"

class TestScene : public Scene {
public:
	BasicShader* basicShader;
	LitShader* litShader;
	ForwardShader* forwardShader;
	Mesh *mesh, *mesh2, *plane, *sphere;
	Material *material, *material2;
	Texture* tex, *tex2, *normalMap;

	Texture* rockAlbedo;
	Texture* rockNormal;
	Texture* rockSpecular;

	void TestScene::Load() {

		std::vector <Vertex> vertices = {
			Vertex(Vector3f(-0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(1, 0, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 0, 1), Vector2f(0, 0)),
		};
		std::vector<unsigned int>indices = {
			0, 1, 2,
		};

		std::vector <Vertex> vertices2 = {
			Vertex(Vector3f(-0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 0)),
			Vertex(Vector3f(0.5f, -0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(1, 0)),
			Vertex(Vector3f(-0.5f, 0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(0, 1)),
			Vertex(Vector3f(0.5f, 0.5f, 0.0f), Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector2f(1, 1)),
		};
		std::vector<unsigned int>indices2 = {
			0, 1, 2, 1, 3, 2
		};

		//mesh = new Mesh(vertices, indices);
		mesh = OBJLoader::loadObj("./res/trooper.obj");
		plane = OBJLoader::loadObj("./res/plane_big.obj");
		sphere = OBJLoader::loadObj("./res/sphere.obj");
		basicShader = new BasicShader("./res/basicShader");
		//litShader = new LitShader("./res/litShader");
		forwardShader = new ForwardShader();
		forwardShader->directionalLightShader->shininess = 200;
		forwardShader->pointLightShader->shininess = 200;
		tex = new Texture("./res/trooper.png", false);
		tex2 = new Texture("./res/grid.png", true);
		normalMap = new Texture("./res/pouch.png", false);

		rockAlbedo = new Texture("./res/rock/albedo.jpg", true);
		rockNormal = new Texture("./res/rock/normal.jpg", false);
		rockSpecular = new Texture("./res/rock/specular.jpg", false);
		Material* rockMaterial = new Material(forwardShader, rockAlbedo, rockNormal);
		rockMaterial->specularMap = rockSpecular;


		Mesh* stevenMesh = OBJLoader::loadObj("./res/steven/steven.obj");
		Texture* stevenAlbedo = new Texture("./res/steven/albedo.png", true);
		Texture* stevenNormal = new Texture("./res/steven/normal.png", false);
		Texture* stevenSpecular = new Texture("./res/steven/specular.png", false);
		Material* stevenMaterial = new Material(forwardShader, stevenAlbedo, stevenNormal);
		rockMaterial->specularMap = stevenSpecular;


		Texture* metalAlbedo = new Texture("./res/metal/albedo.png", true);
		Texture* metalSpecular = new Texture("./res/metal/specular.png", true);
		Material* metalMaterial = new Material(forwardShader, metalSpecular, nullptr);
		metalMaterial->specularMap = metalSpecular;

		material = new Material(forwardShader, tex);
		material->shader = basicShader;

		Texture* gridSpecular = new Texture("./res/grid_specular.png", true);
		material2 = new Material(forwardShader, tex2, nullptr);
		material2->specularMap = gridSpecular;
		material2->shader = basicShader;

		/*	mesh2 = new Mesh(vertices2, indices2);
		tex = new Texture("./res/debug.png");
		material2 = new Material(shader, tex);*/

		GameObject* rig = instanciate("Rig");
		rig->addComponent<TransformerScript>();

		GameObject* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraController>();
		//cam->getComponent<Transform>()->setParent(rig);
		//cam->addComponent<TransformerScript>()->speed = -0.7;

		GameObject* triangle = instanciate("Triangle");
		triangle->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		triangle->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));
		//triangle->getComponent<Transform>()->setScale(Vector3f(1, 1, 1));
		//triangle->getComponent<Transform>()->setParent(rig);
		triangle->addComponent<MeshRenderer>();
		triangle->getComponent<MeshRenderer>()->setMesh(mesh);
		triangle->getComponent<MeshRenderer>()->setMaterial(material);
		triangle->setEnable(false);


		GameObject* steven = instanciate("Steven");
		steven->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		steven->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));
		steven->addComponent<MeshRenderer>();
		steven->getComponent<MeshRenderer>()->setMesh(stevenMesh);
		steven->getComponent<MeshRenderer>()->setMaterial(stevenMaterial);


		/*
		GameObject* ambientLight = instanciate("AmbientLight");
		ambientLight->addComponent<LightEmitter>();
		ambientLight->getComponent<LightEmitter>()->color = Vector3f(0.9f, 0.9f, 1);
		ambientLight->getComponent<LightEmitter>()->intensity = 1;
		*/


		/*
		for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
		for (int k = 0; k < 5; k++) {
		GameObject* pointLight = instanciate("Pointlight" + i);
		//pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(i * 3 - (5/2) * 3, j * 2, k * 3 - (5 / 2) * 3));
		pointLight->getComponent<Transform>()->setParent(rig);
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(i/10.0f, j / 10.0f, k / 10.0f);
		pointLight->getComponent<PointLight>()->constant = 1;
		pointLight->getComponent<PointLight>()->linear = 0.9f;
		pointLight->getComponent<PointLight>()->quadratic = 0.8f;
		}
		}
		}
		*/

		GameObject* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 1.0f;


		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, 1, -1);
		sun->getComponent<DirectionalLight>()->intensity = 0.6f;
		sun->setEnable(false);


		GameObject* pointLight = instanciate("Pointlight");
		pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(6, 2, 0));
		pointLight->getComponent<Transform>()->setParent(rig);
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(0.93f, 0.52f, 0.24f);
		pointLight->getComponent<PointLight>()->constant = 1;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 2;
		//pointLight->setEnable(false);

		GameObject* pointLight2 = instanciate("Pointlight");
		pointLight2->addComponent<PointRenderer>();
		pointLight2->getComponent<Transform>()->setPosition(Vector3f(-6, 4, 0));
		pointLight2->getComponent<Transform>()->setParent(rig);
		pointLight2->addComponent<PointLight>();
		pointLight2->getComponent<PointLight>()->color = Vector3f(0.4f, 0.3f, 1);
		pointLight2->getComponent<PointLight>()->constant = 1;
		pointLight2->getComponent<PointLight>()->linear = 0.09f;
		pointLight2->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight2->getComponent<PointLight>()->intensity = 2;
		//pointLight2->setEnable(false);

		GameObject* quad = instanciate("Quad");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		quad->getComponent<Transform>()->setScale(Vector3f(8, 8, 8));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(material2);
		//quad->addComponent<TransformerScript>();

	}

	void TestScene::OnUnload() {
		delete basicShader;
		delete mesh;
		delete mesh2;
		delete material;
		delete material2;
		delete tex;
		delete tex2;
		delete litShader;
	}

};
#endif // TEST_SCENE_H
