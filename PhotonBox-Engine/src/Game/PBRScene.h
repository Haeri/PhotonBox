#ifndef PBR_SCENE_H
#define PBR_SCENE_H

class BasicShader;
class LitShader;

#include "../Resources/Scene.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Camera.h"
#include "TransformerScript.cpp"
#include "../Resources/OBJLoader.h"
#include "../Components/PointLight.h"
#include "../Resources/BasicShader.h"
#include "../Resources/ForwardShader.h"
#include "../Resources/LitShader.h"
#include "PrinterScript.cpp"
#include "../Components/MeshRenderer.h"
#include "../Components/PointRenderer.h"
#include "CameraController.h"
#include "../Components/AmbientLight.h"

class PBRScene : public Scene {
public:

	void Load() {

		// RESOURCES
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
		Renderer::setSkyBox(new CubeMap(skyBoxLod));

		// OBJ
		Mesh* plane = OBJLoader::loadObj("./res/plane_big.obj");
		Mesh* sphere = OBJLoader::loadObj("./res/sphere.obj");

		// TEXTURES
		Texture* grid = new Texture("./res/grid.png", true);
		Texture* gradient = new Texture("./res/gradient.jpg");
		Texture* woodAlbedo = new Texture("./res/wood/albedo.jpg");
		Texture* woodSpec = new Texture("./res/wood/specular.jpg");
		Texture* woodNormal = new Texture("./res/wood/normal.jpg");
		Texture* woodAo= new Texture("./res/wood/ao.jpg");

		// SHADERS
		BasicShader* basicShader = new BasicShader("./res/basicShader");
		ForwardShader* forwardShader_ = new ForwardShader();
		forwardShader_->directionalLightShader->shininess = 600;
		forwardShader_->pointLightShader->shininess = 600;

		// MATERIALS
		Material* probeMaterial = new Material(basicShader);
		probeMaterial->shader = basicShader;
		probeMaterial->setTexture("specularMap", gradient);
		Material* wood = new Material(basicShader);
		wood->shader = basicShader;
		wood->setTexture("albedoMap", woodAlbedo);
		//wood->specularMap = woodSpec;
		//wood->normalMap = woodNormal;
		//wood->aoMap = woodAo;


		// CAMERA
		GameObject* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraController>();



		// LIGHTS
		GameObject* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 1.0f;

		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, 1, -1);
		sun->getComponent<DirectionalLight>()->intensity = 1.0f;
		//sun->setEnable(false);


		// OBJECTS
		/*
		for (int i = 0; i < 1; ++i) {
		for (int j = 0; j < 1; ++j) {

		ForwardShader* forwardShader = new ForwardShader();
		forwardShader->directionalLightShader->shininess = i + j * 4 + 1;
		forwardShader->pointLightShader->shininess = i + j * 4 + 1;
		material = new Material(forwardShader);
		GameObject* probe = instanciate("Sphere" + i + j);
		probe->getComponent<Transform>()->setPosition(Vector3f(i, 1, j));
		probe->getComponent<Transform>()->setScale(Vector3f(0.3f, 0.3f, 0.3f));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(material);
		}
		}
		*/


		GameObject* probe = instanciate("Sphere");
		probe->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(probeMaterial);

		GameObject* quad = instanciate("Quad");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, -0.2f, 0));
		quad->getComponent<Transform>()->setScale(Vector3f(1, 1, 1));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(wood);
	}

	void OnUnload() {

	}

};
#endif // PBR_SCENE_H
