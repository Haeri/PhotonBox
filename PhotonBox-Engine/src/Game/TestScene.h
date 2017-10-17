#ifndef TEST_SCENE_H
#define TEST_SCENE_H

class BasicShader;
class LitShader;

#include "../Resources/Scene.h"
#include "../Components/Camera.h"
#include "TransformerScript.cpp"
#include "../Resources/OBJLoader.h"
#include "../Components/PointLight.h"
#include "../Resources/BasicShader.h"
#include "../Resources/LitShader.h"
#include "PrinterScript.cpp"
#include "../Components/MeshRenderer.h"
#include "../Components/PointRenderer.h"
#include "CameraController.h"
#include "../Components/AmbientLight.h"
#include "../Resources/SkyBox.h"
#include "../Resources/Texture.h"
#include "../Game/MaterialScript.h"
#include "../Core/PostProcessor.h"
#include "MonochromProcessor.h"
#include "../Resources/PostShader.h"
#include "../Game/BlurShader.h"
#include "../Game/BloomProcessor.h"

class TestScene : public Scene {
public:
	BasicShader* basicShader;
	LitShader* litShader;
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

		//SkyBox
		std::vector<std::string> skyBox = { 
			"./res/skybox/right.jpg",
			"./res/skybox/left.jpg",
			"./res/skybox/top.jpg",
			"./res/skybox/bottom.jpg",
			"./res/skybox/back.jpg",
			"./res/skybox/front.jpg"
		};
		std::vector<std::string> skyBoxNightSpec = {
			"./res/skybox-night/right.png",
			"./res/skybox-night/left.png",
			"./res/skybox-night/top.png",
			"./res/skybox-night/bottom.png",
			"./res/skybox-night/back.png",
			"./res/skybox-night/front.png"
		};
		std::vector<std::string> skyBoxNightDif = {
			"./res/skybox-night_defuse/right.png",
			"./res/skybox-night_defuse/left.png",
			"./res/skybox-night_defuse/top.png",
			"./res/skybox-night_defuse/bottom.png",
			"./res/skybox-night_defuse/back.png",
			"./res/skybox-night_defuse/front.png"
		};
		std::vector<std::string> skyBoxSpec = {
			"./res/neg/right.jpg",
			"./res/neg/left.jpg",
			"./res/neg/top.jpg",
			"./res/neg/bottom.jpg",
			"./res/neg/back.jpg",
			"./res/neg/front.jpg",
		};
		std::vector<std::string> skyBoxLod = {
			"./res/enviroment/lod0_right.jpg",
			"./res/enviroment/lod0_left.jpg",
			"./res/enviroment/lod0_top.jpg",
			"./res/enviroment/lod0_bottom.jpg",
			"./res/enviroment/lod0_back.jpg",
			"./res/enviroment/lod0_front.jpg",

			"./res/enviroment/lod1_right.jpg",
			"./res/enviroment/lod1_left.jpg",
			"./res/enviroment/lod1_top.jpg",
			"./res/enviroment/lod1_bottom.jpg",
			"./res/enviroment/lod1_back.jpg",
			"./res/enviroment/lod1_front.jpg",

			"./res/enviroment/lod2_right.jpg",
			"./res/enviroment/lod2_left.jpg",
			"./res/enviroment/lod2_top.jpg",
			"./res/enviroment/lod2_bottom.jpg",
			"./res/enviroment/lod2_back.jpg",
			"./res/enviroment/lod2_front.jpg",

			"./res/enviroment/lod3_right.jpg",
			"./res/enviroment/lod3_left.jpg",
			"./res/enviroment/lod3_top.jpg",
			"./res/enviroment/lod3_bottom.jpg",
			"./res/enviroment/lod3_back.jpg",
			"./res/enviroment/lod3_front.jpg",
		};


		BloomProcessor* p_bloom = new BloomProcessor(1);


		//PostShader* postShader = new PostShader("./res/post-processing/monochrom");
		//Material* m_monochrom = new Material(postShader);
		//MonochromProcessor* p_monochrom = new MonochromProcessor(2, m_monochrom);

		Renderer::setSkyBox(new CubeMap(skyBoxLod));
		//Renderer::setSkyBox(new CubeMap(skyBoxNightSpec), new CubeMap(skyBoxNightDif));

		//mesh = new Mesh(vertices, indices);
		mesh = OBJLoader::loadObj("./res/trooper.obj");
		plane = OBJLoader::loadObj("./res/plane_big.obj");
		sphere = OBJLoader::loadObj("./res/sphere.obj");
		basicShader = BasicShader::getInstance();
		//litShader = new LitShader("./res/litShader");
		tex = new Texture("./res/trooper.png", false);
		tex2 = new Texture("./res/grid.png", true);
		normalMap = new Texture("./res/pouch.png", false);

		Texture* default_normal = new Texture("./res/default_normal.png", false);
		Texture* default_specular = new Texture("./res/default_specular.png", false);
		Texture* default_emission = new Texture("./res/default_emission.png", false);
		Texture* default_ao = new Texture("./res/default_ao.png", false);

		rockAlbedo = new Texture("./res/rock/albedo.jpg", true);
		rockNormal = new Texture("./res/rock/normal.jpg", false);
		rockSpecular = new Texture("./res/rock/specular.jpg", false);

		Material* rockMaterial = new Material(basicShader);
		rockMaterial->setTexture("albedoMap", rockAlbedo);
		rockMaterial->setTexture("normalMap", rockNormal);
		rockMaterial->setTexture("specularMap", rockSpecular);
		rockMaterial->setProperty<float>("shininess", 300);
		//rockMaterial->shader = basicShader;

		
		//Mesh* stevenMesh = OBJLoader::loadObj("./res/steven/steven.obj");
		//Texture* stevenAlbedo = new Texture("./res/steven/albedo.png", true);
		//Texture* stevenNormal = new Texture("./res/steven/normal.png", false);
		//Texture* stevenSpecular = new Texture("./res/steven/specular.png", false);
		//Material* stevenMaterial = new Material(forwardShader, stevenAlbedo, stevenNormal);
		//stevenMaterial->specularMap = stevenSpecular;
		//stevenMaterial->shader = basicShader;
		


		Mesh* xwingMesh = OBJLoader::loadObj("./res/xwing/x-wing.obj");
		Texture* xwingAlbedo = new Texture("./res/xwing/4k_diffuse.jpg", true);
		Texture* xwingNormal = new Texture("./res/xwing/4k_normal.jpg", false);
		Texture* xwingSpecular = new Texture("./res/xwing/4k_specular.jpg", false);
		Texture* xwingAo = new Texture("./res/xwing/4k_ao.jpg", false);
		Texture* xwingEmission = new Texture("./res/xwing/4k_emission.jpg", false);
		Material* xwingMaterial = new Material(basicShader);
		xwingMaterial->setTexture("albedoMap", xwingAlbedo);
		xwingMaterial->setTexture("normalMap", xwingNormal);
		xwingMaterial->setTexture("specularMap", xwingSpecular);
		xwingMaterial->setTexture("aoMap", xwingAo);
		xwingMaterial->setTexture("emissionMap", xwingEmission);
		xwingMaterial->setProperty<float>("shininess", 900.0f);

		Texture* metalAlbedo = new Texture("./res/metal/albedo.png", true);
		Texture* metalSpecular = new Texture("./res/metal/specular.png", true);
		Material* metalMaterial = new Material(basicShader);
		metalMaterial->setTexture("specularMap", metalSpecular);

		material = new Material(basicShader);
		material->setTexture("albedoMap", tex);
		material->shader = basicShader;

		Texture* gridSpecular = new Texture("./res/grid_specular.png", true);
		Texture* gridEmission = new Texture("./res/grid_emission.png", true);
		material2 = new Material(basicShader);
		material2->setTexture("albedoMap", tex2);
		material2->setTexture("normalMap", default_normal);
		material2->setTexture("specularMap", default_specular);
		material2->setTexture("aoMap", default_ao);
		material2->setTexture("emissionMap", gridEmission);
		material2->setProperty<float>("shininess", 200.0f);
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

		/*GameObject* triangle = instanciate("Triangle");
		triangle->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		triangle->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));*/
		//triangle->getComponent<Transform>()->setScale(Vector3f(1, 1, 1));
		//triangle->getComponent<Transform>()->setParent(rig);
		/*triangle->addComponent<MeshRenderer>();
		triangle->getComponent<MeshRenderer>()->setMesh(mesh);
		triangle->getComponent<MeshRenderer>()->setMaterial(material);*/
		//triangle->setEnable(false);


		/*GameObject* steven = instanciate("Steven");
		steven->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		steven->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));
		steven->addComponent<MeshRenderer>();
		steven->getComponent<MeshRenderer>()->setMesh(stevenMesh);
		steven->getComponent<MeshRenderer>()->setMaterial(stevenMaterial);
		steven->setEnable(false);*/

		GameObject* xwing = instanciate("Steven");
		xwing->getComponent<Transform>()->setPosition(Vector3f(0, 3, 0));
		xwing->getComponent<Transform>()->setScale(Vector3f(3, 3, 3));
		xwing->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));
		xwing->addComponent<MeshRenderer>();
		xwing->getComponent<MeshRenderer>()->setMesh(xwingMesh);
		xwing->getComponent<MeshRenderer>()->setMaterial(xwingMaterial);
		xwing->addComponent<MaterialScript>()->material = xwingMaterial;
		//xwing->setEnable(false);



		/*
		GameObject* ambientLight = instanciate("AmbientLight");
		ambientLight->addComponent<LightEmitter>();
		ambientLight->getComponent<LightEmitter>()->color = Vector3f(0.9f, 0.9f, 1);
		ambientLight->getComponent<LightEmitter>()->intensity = 1;
		*/


		/*
		for (int i = 0; i < 6; i++) {
			for (int j = 1; j < 2; j++) {
				for (int k = 0; k < 6; k++) {
				GameObject* pointLight = instanciate("Pointlight" + std::to_string(i));
				pointLight->addComponent<PointRenderer>();
				pointLight->getComponent<Transform>()->setPosition(Vector3f(i * 10 - (6/2) * 10, j * 10, k * 10 - (6/ 2) * 10));
				pointLight->getComponent<Transform>()->setParent(rig);
				pointLight->addComponent<PointLight>();
				pointLight->getComponent<PointLight>()->color = Vector3f(i/8.0f, j / 2.0f, k / 8.0f);
				pointLight->getComponent<PointLight>()->constant = 1;
				pointLight->getComponent<PointLight>()->linear = 0.09f;
				pointLight->getComponent<PointLight>()->quadratic = 0.032f;
				pointLight->getComponent<PointLight>()->intensity = 3;
				}
			}
		}
		*/

		GameObject* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(29 /255.0f, 52/255.0f, 63/255.0f);
		ambient->getComponent<AmbientLight>()->intensity = 1.0f;


		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(146/255.0f, 253 / 255.0f, 255 / 255.0f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, 1, -1);
		sun->getComponent<DirectionalLight>()->intensity =1.3f;
		//sun->setEnable(false);


		GameObject* pointLight = instanciate("Pointlight");
		pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(8, 1, 0));
		pointLight->getComponent<Transform>()->setParent(rig);
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(0.93f, 0.52f, 0.24f);
		pointLight->getComponent<PointLight>()->constant = 1;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 3;
		pointLight->setEnable(false);

		GameObject* pointLight2 = instanciate("Pointlight");
		pointLight2->addComponent<PointRenderer>();
		pointLight2->getComponent<Transform>()->setPosition(Vector3f(-8, 6, 0));
		pointLight2->getComponent<Transform>()->setParent(rig);
		pointLight2->addComponent<PointLight>();
		pointLight2->getComponent<PointLight>()->color = Vector3f(0.4f, 0.3f, 1);
		pointLight2->getComponent<PointLight>()->constant = 1;
		pointLight2->getComponent<PointLight>()->linear = 0.09f;
		pointLight2->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight2->getComponent<PointLight>()->intensity = 3;
		pointLight2->setEnable(false);

		GameObject* pointLight3 = instanciate("Pointlight");
		pointLight3->addComponent<PointRenderer>();
		pointLight3->getComponent<Transform>()->setPosition(Vector3f(0, 3, 9));
		pointLight3->addComponent<PointLight>();
		pointLight3->getComponent<PointLight>()->color = Vector3f(0.98f, 0.1f, 0.1f);
		pointLight3->getComponent<PointLight>()->constant = 1;
		pointLight3->getComponent<PointLight>()->linear = 0.09f;
		pointLight3->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight3->getComponent<PointLight>()->intensity = 2;
		//pointLight3->setEnable(false);

		GameObject* quad = instanciate("Quad");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		quad->getComponent<Transform>()->setScale(Vector3f(8, 8, 8));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(material2);
		//quad->setEnable(false);
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