#include "TestScene.h"

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
	shader = new Shader("./res/shader");
	forwardShader = new ForwardShader("./res/forward_ambientlight", 
		"./res/forward_pointlight");
	tex = new Texture("./res/trooper.png");
	material = new Material(forwardShader, tex);
	material->shader = shader;
		
/*	mesh2 = new Mesh(vertices2, indices2);
	tex = new Texture("./res/debug.png");
	material2 = new Material(shader, tex);*/

	GameObject* rig = instanciate("Rig");
	rig->addComponent<TransformerScript>();

	GameObject* cam = instanciate("Camera");
	cam->addComponent<Camera>();
	cam->getComponent<Transform>()->setPosition(Vector3f(0, 0, -8));
	cam->getComponent<Transform>()->setParent(rig);

	GameObject* triangle = instanciate("Triangle");
	triangle->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
	triangle->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
	triangle->getComponent<Transform>()->setScale(Vector3f(1, 1, 1));
	triangle->addComponent<MeshRenderer>();
	triangle->getComponent<MeshRenderer>()->setMesh(mesh);
	triangle->getComponent<MeshRenderer>()->setMaterial(material);
	//triangle->addComponent<TransformerScript>();
	//triangle->setEnable(false);

	GameObject* ambientLight = instanciate("AmbientLight");
	ambientLight->addComponent<LightEmitter>();
	ambientLight->getComponent<LightEmitter>()->color = Vector3f(0.9f, 0.9f, 1);
	ambientLight->getComponent<LightEmitter>()->intensity = 1;


	GameObject* pointLight = instanciate("Pointlight");
	pointLight->addComponent<PointLight>();
	pointLight->getComponent<PointLight>()->color = Vector3f(0.9f, 0.9f, 1);
	pointLight->getComponent<PointLight>()->attenuation = 1;
	pointLight->getComponent<PointLight>()->intensity = 1;


/*	GameObject* quad = instanciate("Quad");
	quad->addComponent<MeshRenderer>();
	quad->getComponent<MeshRenderer>()->setMesh(mesh2);
	quad->getComponent<MeshRenderer>()->setMaterial(material2);
	quad->addComponent<TransformerScript>();*/
}

void TestScene::OnUnload(){
	delete shader;
	delete mesh;
	delete mesh2;
	delete material;
	delete material2;
	delete tex;
}
