#ifndef PBR_SCENE_H
#define PBR_SCENE_H

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

#include "../Resources/IrradianceShader.h"

class PBRScene : public Scene {
public:
	CubeMap* sky;

	Mesh* plane;
	Mesh* sphere;

	Texture* woodAlbedo;
	Texture* woodRough;
	Texture* woodNormal;
	Texture* woodAo;
	Texture* woodMetal;

	Texture* bricksAlbedo;
	Texture* bricksRough;
	Texture* bricksNormal;
	Texture* bricksAo;
	Texture* bricksMetal;

	Texture* rustAlbedo;
	Texture* rustRough;
	Texture* rustNormal;
	Texture* rustMetal;

	Texture* goldAlbedo;
	Texture* goldRough;
	Texture* goldNormal;
	Texture* goldMetal;

	Texture* default_normal;
	Texture* default_specular;
	Texture* default_emission;
	Texture* default_ao;

	Material* wood;
	Material* rust;
	Material* bricks;
	Material* gold;
	Material* def;
	Material* lit;




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

		sky = new CubeMap(skyBoxLod2);
		Renderer::setSkyBox(sky);

	
		/* --------------------------- POST PROCESSING --------------------------- */
		BloomProcessor* p_bloom = new BloomProcessor(1);


		/* --------------------------- OBJ --------------------------- */
		plane = OBJLoader::loadObj("./res/plane.obj");
		sphere = OBJLoader::loadObj("./res/sphere.obj");



		/* --------------------------- TEXTURES --------------------------- */
		woodAlbedo = new Texture("./res/materials/mahogfloor/mahogfloor_basecolor.png", true);
		woodRough = new Texture("./res/materials/mahogfloor/mahogfloor_roughness.png", true);
		woodNormal = new Texture("./res/materials/mahogfloor/mahogfloor_normal.png", true);
		woodAo = new Texture("./res/materials/mahogfloor/mahogfloor_AO.png", true);
		woodMetal = new Texture("./res/materials/mahogfloor/mahogfloor_metalness.png", true);

		bricksAlbedo = new Texture("./res/materials/harshbricks/harshbricks-albedo.png", true);
		bricksRough = new Texture("./res/materials/harshbricks/harshbricks-roughness.png", true);
		bricksNormal = new Texture("./res/materials/harshbricks/harshbricks-normal.png", true);
		bricksAo = new Texture("./res/materials/harshbricks/harshbricks-ao.png", true);
		bricksMetal = new Texture("./res/materials/harshbricks/harshbricks-metalness.png", true);

		rustAlbedo = new Texture("./res/materials/rust/rustediron2_basecolor.png", true);
		rustRough = new Texture("./res/materials/rust/rustediron2_roughness.png", true);
		rustNormal = new Texture("./res/materials/rust/rustediron2_normal.png", true);
		rustMetal = new Texture("./res/materials/rust/rustediron2_metallic.png", true);

		goldAlbedo = new Texture("./res/materials/greasy-metal/greasy-metal-pan1-albedo.png", true);
		goldRough = new Texture("./res/materials/greasy-metal/greasy-metal-pan1-roughness.png", true);
		goldNormal = new Texture("./res/materials/greasy-metal/greasy-metal-pan1-normal.png", true);
		goldMetal = new Texture("./res/materials/greasy-metal/greasy-metal-pan1-metal.png", true);

		default_normal = new Texture("./res/default_normal.png", false);
		default_specular = new Texture("./res/default_specular.png", false);
		default_emission = new Texture("./res/default_emission.png", false);
		default_ao = new Texture("./res/default_ao.png", false);

		/* --------------------------- SHADERS --------------------------- */
		LitShader* litShader = LitShader::getInstance();



		/* --------------------------- MATERIALS --------------------------- */
		wood = new Material();
		wood->setTexture("albedoMap", woodAlbedo);
		wood->setTexture("normalMap", woodNormal);
		wood->setTexture("roughnessMap", woodRough);
		wood->setTexture("aoMap", woodAo);
		wood->setTexture("metallicMap", woodMetal);
		wood->setTexture("emissionMap", default_emission);

		rust = new Material();
		rust->setTexture("albedoMap", rustAlbedo);
		rust->setTexture("normalMap", rustNormal);
		rust->setTexture("roughnessMap", rustRough);
		rust->setTexture("aoMap", default_ao);
		rust->setTexture("metallicMap", rustMetal);
		rust->setTexture("emissionMap", default_emission);

		bricks = new Material();
		bricks->setTexture("albedoMap", bricksAlbedo);
		bricks->setTexture("normalMap", bricksNormal);
		bricks->setTexture("roughnessMap", default_emission);
		bricks->setTexture("aoMap", bricksAo);
		bricks->setTexture("metallicMap", bricksMetal);
		bricks->setTexture("emissionMap", default_emission);

		gold = new Material();
		gold->setTexture("albedoMap", goldAlbedo);
		gold->setTexture("normalMap", goldNormal);
		gold->setTexture("roughnessMap", goldRough);
		gold->setTexture("aoMap", default_ao);
		gold->setTexture("metallicMap", goldMetal);
		gold->setTexture("emissionMap", default_emission);

		def = new Material();
		def->setTexture("albedoMap", default_specular);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_ao);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);

		lit = new Material(litShader);
		lit->setProperty("color", Vector3f(0.3, 0.3, 0.5));
		

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
		ambient->getComponent<AmbientLight>()->intensity = 0;

		GameObject* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 2.0f;
		//sun->setEnable(false);

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
		pointLight->getComponent<PointLight>()->intensity = 1.6f;
		pointLight->getComponent<Transform>()->setParent(rig);
		//pointLight->setEnable(false);


		GameObject* probe = instanciate("Probe-1");
		probe->getComponent<Transform>()->setPosition(Vector3f(-6, 1, 0));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(sphere);
		probe->getComponent<MeshRenderer>()->setMaterial(wood);

		GameObject* probe2 = instanciate("Probe-2");
		probe2->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		probe2->addComponent<MeshRenderer>();
		probe2->getComponent<MeshRenderer>()->setMesh(sphere);
		probe2->getComponent<MeshRenderer>()->setMaterial(rust);

		GameObject* probe3 = instanciate("Probe-3");
		probe3->getComponent<Transform>()->setPosition(Vector3f(6, 1, 0));
		probe3->addComponent<MeshRenderer>();
		probe3->getComponent<MeshRenderer>()->setMesh(sphere);
		probe3->getComponent<MeshRenderer>()->setMaterial(bricks);
		
		GameObject* probe4 = instanciate("Probe-4");
		probe4->getComponent<Transform>()->setPosition(Vector3f(12, 1, 0));
		probe4->addComponent<MeshRenderer>();
		probe4->getComponent<MeshRenderer>()->setMesh(sphere);
		probe4->getComponent<MeshRenderer>()->setMaterial(gold);

		GameObject* probe5 = instanciate("Probe-5");
		probe5->getComponent<Transform>()->setPosition(Vector3f(18, 1, 0));
		probe5->addComponent<MeshRenderer>();
		probe5->getComponent<MeshRenderer>()->setMesh(sphere);
		probe5->getComponent<MeshRenderer>()->setMaterial(def);

		GameObject* probe6 = instanciate("Probe-6");
		probe6->getComponent<Transform>()->setPosition(Vector3f(24, 1, 0));
		probe6->addComponent<MeshRenderer>();
		probe6->getComponent<MeshRenderer>()->setMesh(sphere);
		probe6->getComponent<MeshRenderer>()->setMaterial(lit);


		GameObject* quad = instanciate("Quad-1");
		quad->getComponent<Transform>()->setPosition(Vector3f(-6, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(wood);

		GameObject* quad2 = instanciate("Quad-2");
		quad2->getComponent<Transform>()->setPosition(Vector3f(0, 0, -3));
		quad2->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad2->addComponent<MeshRenderer>();
		quad2->getComponent<MeshRenderer>()->setMesh(plane);
		quad2->getComponent<MeshRenderer>()->setMaterial(rust);

		GameObject* quad3 = instanciate("Quad-3");
		quad3->getComponent<Transform>()->setPosition(Vector3f(6, 0, -3));
		quad3->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad3->addComponent<MeshRenderer>();
		quad3->getComponent<MeshRenderer>()->setMesh(plane);
		quad3->getComponent<MeshRenderer>()->setMaterial(bricks);

		GameObject* quad4 = instanciate("Quad-4");
		quad4->getComponent<Transform>()->setPosition(Vector3f(12, 0, -3));
		quad4->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad4->addComponent<MeshRenderer>();
		quad4->getComponent<MeshRenderer>()->setMesh(plane);
		quad4->getComponent<MeshRenderer>()->setMaterial(gold);	

		GameObject* quad5 = instanciate("Quad-5");
		quad5->getComponent<Transform>()->setPosition(Vector3f(18, 0, -3));
		quad5->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad5->addComponent<MeshRenderer>();
		quad5->getComponent<MeshRenderer>()->setMesh(plane);
		quad5->getComponent<MeshRenderer>()->setMaterial(def);

		GameObject* quad6 = instanciate("Quad-6");
		quad6->getComponent<Transform>()->setPosition(Vector3f(24, 0, -3));
		quad6->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad6->addComponent<MeshRenderer>();
		quad6->getComponent<MeshRenderer>()->setMesh(plane);
		quad6->getComponent<MeshRenderer>()->setMaterial(lit);
	}

	void OnUnload() {
		delete sky;

		delete plane;
		delete sphere;

		delete woodAlbedo;
		delete woodRough;
		delete woodNormal;
		delete woodAo;
		delete woodMetal;

		delete bricksAlbedo;
		delete bricksRough;
		delete bricksNormal;
		delete bricksAo;
		delete bricksMetal;

		delete rustAlbedo;
		delete rustRough;
		delete rustNormal;
		delete rustMetal;

		delete goldAlbedo;
		delete goldRough;
		delete goldNormal;
		delete goldMetal;

		delete default_normal;
		delete default_specular;
		delete default_emission;
		delete default_ao;

		delete wood;
		delete rust;
		delete bricks;
		delete gold;
		delete def;
		delete lit;
	}

};
#endif // PBR_SCENE_H