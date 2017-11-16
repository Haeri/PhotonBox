#ifndef TEST_SCENE_H
#define TEST_SCENE_H

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
#include "../Resources/ToneMappingProcessor.h"
#include "../Resources/IrradianceShader.h"
#include "../Resources/Texture.h"

class TestScene : public Scene {
public:
	CubeMap* sky;

	Mesh* plane;
	Mesh* xwingMesh;

	Texture* xwingAlbedo;
	Texture* xwingNormal;
	Texture* xwingSpecular;
	Texture* xwingAo;
	Texture* xwingMetallic;
	Texture* xwingEmission;

	Texture* default_normal;
	Texture* default_specular;
	Texture* default_emission;
	Texture* default_ao;
	Texture* gradient;

	Material* def;
	Material* xwingMaterial;


	void Load() {

		/* --------------------------- RESOURCES --------------------------- */
		std::vector<std::string> skyBoxLod = {
			"./res/enviroment/lod0_right.jpg",
			"./res/enviroment/lod0_left.jpg",
			"./res/enviroment/lod0_top.jpg",
			"./res/enviroment/lod0_bottom.jpg",
			"./res/enviroment/lod0_back.jpg",
			"./res/enviroment/lod0_front.jpg",
		};
		std::vector<std::string> skyBoxLod2 = {
			"./res/dark/posx.jpg",
			"./res/dark/negx.jpg",
			"./res/dark/posy.jpg",
			"./res/dark/negy.jpg",
			"./res/dark/posz.jpg",
			"./res/dark/negz.jpg",
		};
		std::vector<std::string> skyBoxLod3 = {
			"./res/redGreenRoom/lod0_right.jpg",
			"./res/redGreenRoom/lod0_left.jpg",
			"./res/redGreenRoom/lod0_top.jpg",
			"./res/redGreenRoom/lod0_bottom.jpg",
			"./res/redGreenRoom/lod0_back.jpg",
			"./res/redGreenRoom/lod0_front.jpg",
		};
		Renderer::setSkyBox(new CubeMap(skyBoxLod2));


		/* --------------------------- POST PROCESSING --------------------------- */
		BloomProcessor* p_bloom = new BloomProcessor(1);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(2);


		/* --------------------------- OBJ --------------------------- */
		plane = OBJLoader::loadObj("./res/plane.obj");
		xwingMesh = OBJLoader::loadObj("./res/xwing/x-wing.obj");



		/* --------------------------- TEXTURES --------------------------- */
		xwingAlbedo = new Texture("./res/xwing/4k_diffuse.jpg", true);
		xwingNormal = new Texture("./res/xwing/4k_normal.jpg", true);
		xwingSpecular = new Texture("./res/xwing/4k_roughness.jpg", true);
		xwingAo = new Texture("./res/xwing/4k_ao.jpg", true);
		xwingMetallic = new Texture("./res/xwing/4k_emission.jpg", true);
		xwingEmission = new Texture("./res/xwing/4k_emission.jpg", true);

		default_normal = new Texture("./res/default_normal.png", false);
		default_specular = new Texture("./res/default_specular.png", false);
		default_emission = new Texture("./res/default_emission.png", false);
		default_ao = new Texture("./res/default_ao.png", false);
		gradient = new Texture("./res/gradient.png", false);



		/* --------------------------- SHADERS --------------------------- */
		LitShader* litShader = LitShader::getInstance();



		/* --------------------------- MATERIALS --------------------------- */
		xwingMaterial = new Material();
		xwingMaterial->setTexture("albedoMap", xwingAlbedo);
		xwingMaterial->setTexture("normalMap", xwingNormal);
		xwingMaterial->setTexture("roughnessMap", xwingSpecular);
		xwingMaterial->setTexture("aoMap", xwingAo);
		xwingMaterial->setTexture("metallicMap", default_emission);
		xwingMaterial->setTexture("emissionMap", xwingEmission);

		def = new Material();
		def->setTexture("albedoMap", default_specular);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_specular);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);


		/* --------------------------- CAMERA --------------------------- */
		GameObject* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraController>();
		cam->addComponent<MaterialScript>()->material = p_tonemapping->material;



		/* --------------------------- LIGHTS --------------------------- */
		GameObject* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;

		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 2.0f;
		sun->setEnable(false);

		GameObject* rig = instanciate("Rig");
		rig->addComponent<TransformerScript>();

		GameObject* pointLight = instanciate("Pointlight");
		pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(-7, 2, 0));
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(165 / 255.0f, 249 / 255.0f, 245 / 255.0f);
		pointLight->getComponent<PointLight>()->constant = 2;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 3.0f;
		pointLight->getComponent<Transform>()->setParent(rig);
		//pointLight->setEnable(false);

		GameObject* pointLight2 = instanciate("Pointlight2");
		pointLight2->addComponent<PointRenderer>();
		pointLight2->getComponent<Transform>()->setPosition(Vector3f(0, 3, 9));
		pointLight2->addComponent<PointLight>();
		pointLight2->getComponent<PointLight>()->color = Vector3f(255 / 255.0f, 30 / 255.0f, 50 / 255.0f);
		pointLight2->getComponent<PointLight>()->constant = 2;
		pointLight2->getComponent<PointLight>()->linear = 0.09f;
		pointLight2->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight2->getComponent<PointLight>()->intensity = 4.0f;


		GameObject* xwing = instanciate("X-Wing");
		xwing->getComponent<Transform>()->setPosition(Vector3f(0, 3, 0));
		xwing->getComponent<Transform>()->setScale(Vector3f(3, 3, 3));
		xwing->getComponent<Transform>()->setRotation(Vector3f(0, 3.1415, 0));
		xwing->addComponent<MeshRenderer>();
		xwing->getComponent<MeshRenderer>()->setMesh(xwingMesh);
		xwing->getComponent<MeshRenderer>()->setMaterial(xwingMaterial);

		GameObject* floor = instanciate("Floor");
		floor->getComponent<Transform>()->setScale(Vector3f(100, 100, 100));
		floor->addComponent<MeshRenderer>();
		floor->getComponent<MeshRenderer>()->setMesh(plane);
		floor->getComponent<MeshRenderer>()->setMaterial(def);
	}

	void OnUnload() {
		delete sky;

		delete plane;
		delete xwingMesh;

		delete xwingAlbedo;
		delete xwingNormal;
		delete xwingSpecular;
		delete xwingAo;
		delete xwingMetallic;
		delete xwingEmission;

		delete default_normal;
		delete default_specular;
		delete default_emission;
		delete default_ao;
		delete gradient;

		delete def;
		delete xwingMaterial;
	}

};
#endif // TEST_SCENE_H