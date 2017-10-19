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
#include "../Resources/LitShader.h"
#include "PrinterScript.cpp"
#include "../Components/MeshRenderer.h"
#include "../Components/PointRenderer.h"
#include "CameraController.h"
#include "../Components/AmbientLight.h"
#include "../Game/MaterialScript.h"
#include "../Game/BlurShader.h"
#include "../Game/BloomProcessor.h"
#include "../Game/BlurProcessor.h"

class PBRScene : public Scene {
public:

	void Load() {

		/* --------------------------- RESOURCES --------------------------- */
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

	
		/* --------------------------- POST PROCESSING --------------------------- */
		BloomProcessor* p_bloom = new BloomProcessor(1);
		//BlurProcessor* p_bloom = new BlurProcessor(1);
		//MonochromProcessor* p_monochrom = new MonochromProcessor(2);



		/* --------------------------- OBJ --------------------------- */
		Mesh* plane = OBJLoader::loadObj("./res/plane_big.obj");
		Mesh* sphere = OBJLoader::loadObj("./res/sphere.obj");



		/* --------------------------- TEXTURES --------------------------- */
		Texture* grid = new Texture("./res/grid.png", true);
		Texture* gradient = new Texture("./res/gradient.jpg");

		Texture* woodFloorAlbedo = new Texture("./res/materials/mahogfloor/mahogfloor_basecolor.png", true);
		Texture* woodFloorNormal = new Texture("./res/materials/mahogfloor/mahogfloor_normal.png", true);
		Texture* woodFloorAo = new Texture("./res/materials/mahogfloor/mahogfloor_AO.png", true);


		Texture* default_normal = new Texture("./res/default_normal.png", false);
		Texture* default_specular = new Texture("./res/default_specular.png", false);
		Texture* default_emission = new Texture("./res/default_emission.png", false);
		Texture* default_ao = new Texture("./res/default_ao.png", false);

		/* --------------------------- SHADERS --------------------------- */


		/* --------------------------- MATERIALS --------------------------- */
		Material* probeMaterial = new Material();
		probeMaterial->setTexture("albedoMap", grid);
		probeMaterial->setTexture("normalMap", default_normal);
		probeMaterial->setTexture("specularMap", default_specular);
		probeMaterial->setTexture("aoMap", default_ao);
		probeMaterial->setProperty("shininess", 2000.0f);

		Material* wood = new Material();
		wood->setTexture("albedoMap", woodFloorAlbedo);
		wood->setTexture("normalMap", woodFloorNormal);
		wood->setTexture("specularMap", default_specular);
		wood->setTexture("aoMap", woodFloorAo);
		wood->setProperty("shininess", 1200.0f);


		/* --------------------------- CAMERA --------------------------- */
		GameObject* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraController>();



		/* --------------------------- LIGHTS --------------------------- */
		GameObject* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 1.0f;

		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, 1, -1);
		sun->getComponent<DirectionalLight>()->intensity = 1.0f;
		sun->setEnable(false);


		GameObject* pointLight = instanciate("Pointlight");
		pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(-3, 1, 0));
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(245/255.0f, 249 / 255.0f, 165 / 255.0f);
		pointLight->getComponent<PointLight>()->constant = 1;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 3;


		GameObject* probe = instanciate("Sphere");
		probe->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(probeMaterial);
		probe->addComponent<MaterialScript>()->material = probeMaterial;

		GameObject* quad = instanciate("Quad");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		quad->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(wood);
	}

	void OnUnload() {

	}

};
#endif // PBR_SCENE_H