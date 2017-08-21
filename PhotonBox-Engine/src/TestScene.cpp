#include "TestScene.h"
#include "Resources/BasicShader.h"
#include "Resources/ForwardShader.h"
#include "PrinterScript.cpp"
#include "Components/MeshRenderer.h"
#include "Components/PointRenderer.h"

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
	plane = OBJLoader::loadObj("./res/plane.obj");
	sphere = OBJLoader::loadObj("./res/sphere.obj");
	basicShader = new BasicShader("./res/basicShader");
	forwardShader = new ForwardShader("./res/forward_ambientlight", 
		"./res/forward_pointlight");
	tex = new Texture("./res/trooper.png");
	tex2 = new Texture("./res/grid.png");
	material = new Material(forwardShader, tex);
	material->shader = basicShader;

	material2 = new Material(forwardShader, tex2);
	material2->shader = basicShader;
		
/*	mesh2 = new Mesh(vertices2, indices2);
	tex = new Texture("./res/debug.png");
	material2 = new Material(shader, tex);*/

	GameObject* rig = instanciate("Rig");
	rig->addComponent<TransformerScript>();

	GameObject* cam = instanciate("Camera");
	cam->addComponent<Camera>();
	cam->getComponent<Transform>()->setPosition(Vector3f(0, 0, -30 ));
	//cam->getComponent<Transform>()->setParent(rig);
	cam->addComponent<PrinterScript>();


	GameObject* triangle = instanciate("Triangle");
	triangle->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
	triangle->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));
	triangle->getComponent<Transform>()->setScale(Vector3f(1, 1, 1));
	//triangle->getComponent<Transform>()->setParent(rig);
	triangle->addComponent<MeshRenderer>();
	triangle->getComponent<MeshRenderer>()->setMesh(mesh);
	triangle->getComponent<MeshRenderer>()->setMaterial(material);
	//triangle->addComponent<TransformerScript>();
	//triangle->setEnable(false);

	/*
	GameObject* ambientLight = instanciate("AmbientLight");
	ambientLight->addComponent<LightEmitter>();
	ambientLight->getComponent<LightEmitter>()->color = Vector3f(0.9f, 0.9f, 1);
	ambientLight->getComponent<LightEmitter>()->intensity = 1;
	*/

	GameObject* pointLight = instanciate("Pointlight");
	pointLight->addComponent<MeshRenderer>()->setMesh(sphere);
	pointLight->getComponent<MeshRenderer>()->setMaterial(material2);
	//pointLight->addComponent<PointRenderer>();
	pointLight->getComponent<Transform>()->setPosition(Vector3f(-2, -1, 3));
	pointLight->getComponent<Transform>()->setScale(Vector3f(0.1, 0.1, 0.1));
	pointLight->getComponent<Transform>()->setParent(rig);
	pointLight->addComponent<PointLight>();
	pointLight->getComponent<PointLight>()->color = Vector3f(0.5f, 0.86f, 0.96f);
	pointLight->getComponent<PointLight>()->constant = 1;
	pointLight->getComponent<PointLight>()->linear = 0.09f;
	pointLight->getComponent<PointLight>()->quadratic = 0.032f;
	//pointLight->addComponent<PrinterScript>();


	GameObject* pointLight2 = instanciate("Pointlight");
	pointLight2->addComponent<MeshRenderer>()->setMesh(sphere);
	pointLight2->getComponent<MeshRenderer>()->setMaterial(material2);
	//pointLight2->addComponent<PointRenderer>();
	pointLight2->getComponent<Transform>()->setPosition(Vector3f(2, 0, -4));
	pointLight2->getComponent<Transform>()->setScale(Vector3f(0.1, 0.1, 0.1));
	pointLight2->getComponent<Transform>()->setParent(rig);
	pointLight2->addComponent<PointLight>();
	pointLight2->getComponent<PointLight>()->color = Vector3f(0.99f, 0.9f, 0.87f);
	pointLight2->getComponent<PointLight>()->constant = 1;
	pointLight2->getComponent<PointLight>()->linear = 0.09f;
	pointLight2->getComponent<PointLight>()->quadratic = 0.032f;
	pointLight2->getComponent<PointLight>()->intensity = 0.7f;
	//pointLight2->addComponent<PrinterScript>();


	GameObject* quad = instanciate("Quad");
	quad->getComponent<Transform>()->setPosition(Vector3f(0, -1, 0));
	quad->getComponent<Transform>()->setScale(Vector3f(30, 30, 30));
	quad->addComponent<MeshRenderer>();
	quad->getComponent<MeshRenderer>()->setMesh(plane);
	quad->getComponent<MeshRenderer>()->setMaterial(material2);
	//quad->addComponent<TransformerScript>();
}

void TestScene::OnUnload(){
	delete basicShader;
	delete mesh;
	delete mesh2;
	delete material;
	delete material2;
	delete tex;
	delete tex2;
}
