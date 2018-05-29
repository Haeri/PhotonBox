#ifndef PBR_SCENE_H
#define PBR_SCENE_H

#include <Components/AmbientLight.h>
#include <Components/Camera.h>
#include <Components/MeshRenderer.h>
#include <Components/MeshRenderer.h>
#include <Components/PointLight.h>
#include <Components/PointRenderer.h>
#include <Components/TransparentMeshRenderer.h>
#include <Resources/BasicShader.h>
#include <Resources/IrradianceShader.h>
#include <Resources/LitShader.h>
#include <Resources/OBJLoader.h>
#include <Resources/Scene.h>
#include <Resources/TransparentShader.h>

#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../PostProcessors/BlurProcessor.cpp"
#include "../Shader/BlurShader.cpp"
#include "../Scripts/StateControllerScript.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/MaterialScript.cpp"
#include "../Scripts/PrinterScript.cpp"
#include "../Scripts/TransformerScript.cpp"

class PBRScene : public Scene
{
public:
	CubeMap * sky;

	Mesh* plane;
	Mesh* sphere;
	Mesh* cube;
	Mesh* couchMesh;
	Mesh* car;
	Mesh* tree_branch_mesh;
	Mesh* tree_leaves_mesh;

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

	Texture* bark;
	Texture* leaveAlbedo;
	Texture* leaveNormal;
	Texture* leaveRoughness;

	Texture* default_normal;
	Texture* default_specular;
	Texture* default_emission;
	Texture* default_ao;
	Texture* gradient;
	Texture* default_roughness;
	Texture* transparentAlbedo;

	Material* wood;
	Material* rust;
	Material* bricks;
	Material* gold;
	Material* def;
	Material* lit;
	Material* glassMaterial;
	Material* barkMaterial;
	Material* leaveMaterial;

	AutoExposureProcessor* p_autoExposure;
	BloomProcessor* p_bloom;
	ToneMappingProcessor* p_tonemapping;
	SSAOProcessor* p_ssao;
	SSReflectionProcessor* p_ssreflection;

	void Load()
	{

		/* --------------------------- RESOURCES --------------------------- */
		std::vector<std::string> nightSky = {
			"./res/enviroment/dark/posx.jpg",
			"./res/enviroment/dark/negx.jpg",
			"./res/enviroment/dark/posy.jpg",
			"./res/enviroment/dark/negy.jpg",
			"./res/enviroment/dark/posz.jpg",
			"./res/enviroment/dark/negz.jpg",
		};

		sky = new CubeMap(nightSky);
		Renderer::setSkyBox(sky);
		Renderer::getSkyBox()->intensity = 1;


		/* --------------------------- POST PROCESSING --------------------------- */
		p_ssao = new SSAOProcessor(0);
		p_ssreflection = new SSReflectionProcessor(1);
		p_autoExposure = new AutoExposureProcessor(2);
		p_bloom = new BloomProcessor(3);
		p_tonemapping = new ToneMappingProcessor(4);


		/* --------------------------- OBJ --------------------------- */
		plane = OBJLoader::loadObj(Resources::ENGINE_RESOURCES + "/primitives/plane.obj");
		sphere = OBJLoader::loadObj(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");
		cube = OBJLoader::loadObj(Resources::ENGINE_RESOURCES + "/primitives/cube.obj");
		couchMesh = OBJLoader::loadObj("./res/Realistic-Rendering/Couch/couch.obj");
		car = OBJLoader::loadObj("./res/meshes/car.obj");
		tree_branch_mesh = OBJLoader::loadObj("./res/collection/Tree/Tree_Branch.obj");
		tree_leaves_mesh = OBJLoader::loadObj("./res/collection/Tree/Leaves.obj");



		/* --------------------------- TEXTURES --------------------------- */
		woodAlbedo = new Texture(std::string("./res/materials/mahogfloor/mahogfloor_basecolor.png"), true);
		woodRough = new Texture(std::string("./res/materials/mahogfloor/mahogfloor_roughness.png"), true);
		woodNormal = new Texture(std::string("./res/materials/mahogfloor/mahogfloor_normal.png"), true);
		woodAo = new Texture(std::string("./res/materials/mahogfloor/mahogfloor_AO.png"), true);
		woodMetal = new Texture(std::string("./res/materials/mahogfloor/mahogfloor_metalness.png"), true);

		bricksAlbedo = new Texture(std::string("./res/materials/harshbricks/harshbricks-albedo.png"), true);
		bricksRough = new Texture(std::string("./res/materials/harshbricks/harshbricks-roughness.png"), true);
		bricksNormal = new Texture(std::string("./res/materials/harshbricks/harshbricks-normal.png"), true);
		bricksAo = new Texture(std::string("./res/materials/harshbricks/harshbricks-ao.png"), true);
		bricksMetal = new Texture(std::string("./res/materials/harshbricks/harshbricks-metalness.png"), true);

		rustAlbedo = new Texture(std::string("./res/materials/rust/rustediron2_basecolor.png"), true);
		rustRough = new Texture(std::string("./res/materials/rust/rustediron2_roughness.png"), true);
		rustNormal = new Texture(std::string("./res/materials/rust/rustediron2_normal.png"), true);
		rustMetal = new Texture(std::string("./res/materials/rust/rustediron2_metallic.png"), true);

		goldAlbedo = new Texture(std::string("./res/materials/greasy-metal/greasy-metal-pan1-albedo.png"), true);
		goldRough = new Texture(std::string("./res/materials/greasy-metal/greasy-metal-pan1-roughness.png"), true);
		goldNormal = new Texture(std::string("./res/materials/greasy-metal/greasy-metal-pan1-normal.png"), true);
		goldMetal = new Texture(std::string("./res/materials/greasy-metal/greasy-metal-pan1-metal.png"), true);

		bark = new Texture(std::string("./res/collection/Tree/bark_0021.jpg"), true);
		leaveAlbedo = new Texture(std::string("./res/collection/Tree/DB2X2_L01.png"), true);
		leaveRoughness = new Texture(std::string("./res/collection/Tree/DB2X2_L01_Spec.png"), true);
		leaveNormal = new Texture(std::string("./res/collection/Tree/DB2X2_L01_Nor.png"), true);

		default_normal = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		default_specular = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		default_emission = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		default_ao = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		default_roughness = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		gradient = new Texture(std::string(Resources::ENGINE_RESOURCES + "/gradient.png"), false);
		transparentAlbedo = new Texture(std::string("./res/Realistic-Rendering/Window/albedo.png"), true);

		/* --------------------------- SHADERS --------------------------- */
		LitShader* litShader = LitShader::getInstance();
		TransparentShader* transparentShader = TransparentShader::getInstance();



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
		bricks->setTexture("roughnessMap", bricksRough);
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

		glassMaterial = new Material(transparentShader);
		glassMaterial->setTexture("albedoMap", transparentAlbedo);
		glassMaterial->setTexture("normalMap", default_normal);
		glassMaterial->setTexture("roughnessMap", default_roughness);
		glassMaterial->setTexture("aoMap", default_ao);
		glassMaterial->setTexture("metallicMap", default_emission);
		glassMaterial->setTexture("emissionMap", default_emission);

		barkMaterial = new Material();
		barkMaterial->setTexture("albedoMap", bark);
		barkMaterial->setTexture("normalMap", default_normal);
		barkMaterial->setTexture("roughnessMap", default_roughness);
		barkMaterial->setTexture("aoMap", default_ao);
		barkMaterial->setTexture("metallicMap", default_emission);
		barkMaterial->setTexture("emissionMap", default_emission);

		leaveMaterial = new Material();
		leaveMaterial->setTexture("albedoMap", leaveAlbedo);
		leaveMaterial->setTexture("normalMap", leaveNormal);
		leaveMaterial->setTexture("roughnessMap", leaveRoughness);
		leaveMaterial->setTexture("aoMap", default_ao);
		leaveMaterial->setTexture("metallicMap", default_emission);
		leaveMaterial->setTexture("emissionMap", default_emission);

		lit = new Material(litShader);
		lit->setProperty("color", Vector3f(0.3, 0.3, 0.5));


		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<StateControllerScript>();
		cam->addComponent<CameraControllerScript>();


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;

		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 2.0f;
		//sun->setEnable(false);

		Entity* rig = instanciate("Rig");
		rig->addComponent<TransformerScript>();

		Entity* pointLight = instanciate("Pointlight");
		//pointLight->addComponent<PointRenderer>();
		pointLight->getComponent<Transform>()->setPosition(Vector3f(-7, 2, 0));
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(165 / 255.0f, 249 / 255.0f, 245 / 255.0f);
		pointLight->getComponent<PointLight>()->constant = 2;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 1.6f;
		pointLight->getComponent<Transform>()->setParent(rig);
		//pointLight->setEnable(false);

		
		Entity* probe = instanciate("Probe-1");
		//probe->getComponent<Transform>()->setScale(Vector3f(3, 3, 3));
		probe->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		probe->addComponent<MeshRenderer>();
		probe->getComponent<MeshRenderer>()->setMesh(car);
		probe->getComponent<MeshRenderer>()->setMaterial(rust);

		
		Entity* floor = instanciate("Floor");
		floor->getComponent<Transform>()->setScale(Vector3f(10, 10, 10));
		floor->addComponent<MeshRenderer>();
		floor->getComponent<MeshRenderer>()->setMesh(plane);
		floor->getComponent<MeshRenderer>()->setMaterial(def);

		/*
		Entity* tree_branch = instanciate("Branch");
		tree_branch->addComponent<MeshRenderer>()->setRenderType(RenderType::cutout);
		tree_branch->getComponent<MeshRenderer>()->setMesh(tree_branch_mesh);
		tree_branch->getComponent<MeshRenderer>()->setMaterial(barkMaterial);

		Entity* leaves = instanciate("Leaves");
		leaves->addComponent<MeshRenderer>();
		leaves->getComponent<MeshRenderer>()->setMesh(tree_leaves_mesh);
		leaves->getComponent<MeshRenderer>()->setMaterial(leaveMaterial);
		*/

			/*
		Entity* couch = instanciate("Couch");
		couch->addComponent<MeshRenderer>();
		couch->getComponent<MeshRenderer>()->setMesh(couchMesh);
		couch->getComponent<MeshRenderer>()->setMaterial(wood);

		/*
		Entity* probe2 = instanciate("Probe-2");
		probe2->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		probe2->addComponent<MeshRenderer>();
		probe2->getComponent<MeshRenderer>()->setMesh(sphere);
		probe2->getComponent<MeshRenderer>()->setMaterial(rust);

		Entity* quad = instanciate("Quad-1");
		quad->getComponent<Transform>()->setPosition(Vector3f(-6, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(wood);
		*/
		/*
		Entity* probe3 = instanciate("Probe-3");
		probe3->getComponent<Transform>()->setPosition(Vector3f(6, 1, 0));
		probe3->addComponent<MeshRenderer>();
		probe3->getComponent<MeshRenderer>()->setMesh(sphere);
		probe3->getComponent<MeshRenderer>()->setMaterial(bricks);

		Entity* probe4 = instanciate("Probe-4");
		probe4->getComponent<Transform>()->setPosition(Vector3f(12, 1, 0));
		probe4->addComponent<MeshRenderer>();
		probe4->getComponent<MeshRenderer>()->setMesh(sphere);
		probe4->getComponent<MeshRenderer>()->setMaterial(gold);

		Entity* probe5 = instanciate("Probe-5");
		probe5->getComponent<Transform>()->setPosition(Vector3f(18, 1, 0));
		probe5->addComponent<MeshRenderer>();
		probe5->getComponent<MeshRenderer>()->setMesh(sphere);
		probe5->getComponent<MeshRenderer>()->setMaterial(def);

		Entity* probe6 = instanciate("Probe-6");
		probe6->getComponent<Transform>()->setPosition(Vector3f(24, 1, 0));
		probe6->addComponent<MeshRenderer>();
		probe6->getComponent<MeshRenderer>()->setMesh(sphere);
		probe6->getComponent<MeshRenderer>()->setMaterial(lit);

		Entity* probe7 = instanciate("Probe-7");
		probe7->getComponent<Transform>()->setPosition(Vector3f(30, 1, 0));
		probe7->addComponent<TransparentMeshRenderer>();
		probe7->getComponent<TransparentMeshRenderer>()->setMesh(sphere);
		probe7->getComponent<TransparentMeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe8 = instanciate("Probe-7");
		probe8->getComponent<Transform>()->setPosition(Vector3f(36, 1, 0));
		probe8->addComponent<TransparentMeshRenderer>();
		probe8->getComponent<TransparentMeshRenderer>()->setMesh(sphere);
		probe8->getComponent<TransparentMeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe9 = instanciate("Probe-7");
		probe9->getComponent<Transform>()->setPosition(Vector3f(42, 1, 0));
		probe9->addComponent<TransparentMeshRenderer>();
		probe9->getComponent<TransparentMeshRenderer>()->setMesh(sphere);
		probe9->getComponent<TransparentMeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe10 = instanciate("Probe-7");
		probe10->getComponent<Transform>()->setPosition(Vector3f(48, 1, 0));
		probe10->addComponent<TransparentMeshRenderer>();
		probe10->getComponent<TransparentMeshRenderer>()->setMesh(sphere);
		probe10->getComponent<TransparentMeshRenderer>()->setMaterial(glassMaterial);



		Entity* quad = instanciate("Quad-1");
		quad->getComponent<Transform>()->setPosition(Vector3f(-6, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(plane);
		quad->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* quad2 = instanciate("Quad-2");
		quad2->getComponent<Transform>()->setPosition(Vector3f(0, 0, -3));
		quad2->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad2->addComponent<MeshRenderer>();
		quad2->getComponent<MeshRenderer>()->setMesh(plane);
		quad2->getComponent<MeshRenderer>()->setMaterial(rust);

		Entity* quad3 = instanciate("Quad-3");
		quad3->getComponent<Transform>()->setPosition(Vector3f(6, 0, -3));
		quad3->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad3->addComponent<MeshRenderer>();
		quad3->getComponent<MeshRenderer>()->setMesh(plane);
		quad3->getComponent<MeshRenderer>()->setMaterial(bricks);

		Entity* quad4 = instanciate("Quad-4");
		quad4->getComponent<Transform>()->setPosition(Vector3f(12, 0, -3));
		quad4->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad4->addComponent<MeshRenderer>();
		quad4->getComponent<MeshRenderer>()->setMesh(plane);
		quad4->getComponent<MeshRenderer>()->setMaterial(gold);

		Entity* quad5 = instanciate("Quad-5");
		quad5->getComponent<Transform>()->setPosition(Vector3f(18, 0, -3));
		quad5->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad5->addComponent<MeshRenderer>();
		quad5->getComponent<MeshRenderer>()->setMesh(plane);
		quad5->getComponent<MeshRenderer>()->setMaterial(def);

		Entity* quad6 = instanciate("Quad-6");
		quad6->getComponent<Transform>()->setPosition(Vector3f(24, 0, -3));
		quad6->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad6->addComponent<MeshRenderer>();
		quad6->getComponent<MeshRenderer>()->setMesh(plane);
		quad6->getComponent<MeshRenderer>()->setMaterial(lit);

		Entity* quad7 = instanciate("Quad-7");
		quad7->getComponent<Transform>()->setPosition(Vector3f(30, 0, -3));
		quad7->getComponent<Transform>()->setScale(Vector3f(2, 2, 2));
		quad7->addComponent<TransparentMeshRenderer>();
		quad7->getComponent<TransparentMeshRenderer>()->setMesh(plane);
		quad7->getComponent<TransparentMeshRenderer>()->setMaterial(glassMaterial);
		*/
	}

	void OnUnload()
	{
		delete sky;

		delete plane;
		delete sphere;
		delete cube;

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
		delete gradient;
		delete default_roughness;
		delete transparentAlbedo;

		delete wood;
		delete rust;
		delete bricks;
		delete gold;
		delete def;
		delete lit;
		delete glassMaterial;

		//delete p_autoExposure;
		//delete p_bloom;
		//delete p_tonemapping;
		//delete p_ssao;
		//delete p_ssreflection;
	}

};
#endif // PBR_SCENE_H
