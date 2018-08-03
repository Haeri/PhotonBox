#ifndef PBR_SCENE_CPP
#define PBR_SCENE_CPP

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
#include <resources/GShader.h>

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
	void Load()
	{
		/* --------------------------- RESOURCES --------------------------- */
		/*
		std::vector<std::string> nightSky = {
			"./res/enviroment/dark/posx.jpg",
			"./res/enviroment/dark/negx.jpg",
			"./res/enviroment/dark/posy.jpg",
			"./res/enviroment/dark/negy.jpg",
			"./res/enviroment/dark/posz.jpg",
			"./res/enviroment/dark/negz.jpg",
		};
		*/

		std::vector<std::string> studio = {
			"./res/enviroment/studio/face-l.jpg",
			"./res/enviroment/studio/face-r.jpg",
			"./res/enviroment/studio/face-t.jpg",
			"./res/enviroment/studio/face-b.jpg",
			"./res/enviroment/studio/face-d.jpg",
			"./res/enviroment/studio/face-f.jpg",
		};

		//Renderer::setSkyBox(createResource<CubeMap>(nightSky));
		Renderer::setSkyBox(new CubeMap(studio));
		Renderer::getSkyBox()->intensity = 3;

		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor* p_ssao = new SSAOProcessor(0);
		SSReflectionProcessor* p_ssreflection = new SSReflectionProcessor(1);
		AutoExposureProcessor* p_autoExposure = new AutoExposureProcessor(2);
		BloomProcessor* p_bloom = new BloomProcessor(3);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(4);

		/* --------------------------- OBJ --------------------------- */
		Mesh* plane = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/plane.obj");
		//Mesh* sphere = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");
		//Mesh* cube = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/cube.obj");
		//Mesh* couchMesh = createResource<Mesh>("./res/Realistic-Rendering/Couch/couch.obj");
		//Mesh* car = createResource<Mesh>("./res/meshes/rallyCarHigh.obj");
		Mesh* shelby_chassis = createResource<Mesh>("./res/meshes/shelby/shelby_chassis.obj");
		Mesh* shelby_chrome = createResource<Mesh>("./res/meshes/shelby/shelby_chrome.obj");
		Mesh* shelby_tires = createResource<Mesh>("./res/meshes/shelby/shelby_tires.obj");
		//Mesh* tree_branch_mesh = createResource<Mesh>("./res/collection/Tree/Tree_Branch.obj");
		//Mesh* tree_leaves_mesh = createResource<Mesh>("./res/collection/Tree/Leaves.obj");

		/* --------------------------- TEXTURES --------------------------- */
		/*Texture* woodAlbedo = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_basecolor.png"), true);
		Texture* woodRough = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_roughness.png"), true);
		Texture* woodNormal = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_normal.png"), true);
		Texture* woodAo = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_AO.png"), true);
		Texture* woodMetal = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_metalness.png"), true);
		
		Texture* bricksAlbedo = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-albedo.png"), true);
		Texture* bricksRough = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-roughness.png"), true);
		Texture* bricksNormal = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-normal.png"), true);
		Texture* bricksAo = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-ao.png"), true);
		Texture* bricksMetal = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-metalness.png"), true);*/
		
		Texture* rustAlbedo = createResource<Texture>(std::string("./res/materials/rust/rustediron2_basecolor.png"), true);
		Texture* rustRough = createResource<Texture>(std::string("./res/materials/rust/rustediron2_roughness.png"), true);
		Texture* rustNormal = createResource<Texture>(std::string("./res/materials/rust/rustediron2_normal.png"), true);
		Texture* rustMetal = createResource<Texture>(std::string("./res/materials/rust/rustediron2_metallic.png"), true);
		

		Texture* aluminium_a = createResource<Texture>(std::string("./res/materials/aluminium/albedo.png"), true);
		Texture* aluminium_r = createResource<Texture>(std::string("./res/materials/aluminium/roughness.png"), true);
		Texture* aluminium_n = createResource<Texture>(std::string("./res/materials/aluminium/normal.png"), true);
		Texture* aluminium_m = createResource<Texture>(std::string("./res/materials/aluminium/metallic.png"), true);

		Texture* tire_text = createResource<Texture>(std::string("./res/meshes/shelby/tire.png"), true);

		Texture* red = createResource<Texture>(std::string("./res/meshes/shelby/red.png"));
		
		/*Texture* goldAlbedo = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-albedo.png"), true);
		Texture* goldRough = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-roughness.png"), true);
		Texture* goldNormal = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-normal.png"), true);
		Texture* goldMetal = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-metal.png"), true);
		
		Texture* bark = createResource<Texture>(std::string("./res/collection/Tree/bark_0021.jpg"), true);
		Texture* leaveAlbedo = createResource<Texture>(std::string("./res/collection/Tree/DB2X2_L01.png"), true);
		Texture* leaveRoughness = createResource<Texture>(std::string("./res/collection/Tree/DB2X2_L01_Spec.png"), true);
		Texture* leaveNormal = createResource<Texture>(std::string("./res/collection/Tree/DB2X2_L01_Nor.png"), true);*/
		 
		Texture* default_normal = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		Texture* default_roughness = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		Texture* default_gray = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_gray18.png"), false);
		Texture* gradient = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/gradient.png"), false);
		Texture* transparentAlbedo = createResource<Texture>(std::string("./res/Realistic-Rendering/Window/albedo.png"), true);
		Texture* grid = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/grid.png"), false, false);


		/* --------------------------- SHADERS --------------------------- */
		LitShader* litShader = LitShader::getInstance();
		TransparentShader* transparentShader = TransparentShader::getInstance();
		GShader* defaultShader = GShader::getInstance();



		/* --------------------------- MATERIALS --------------------------- */
		//Material* wood = createResource<Material>(defaultShader);
		//wood->setTexture("albedoMap", woodAlbedo);
		//wood->setTexture("normalMap", woodNormal);
		//wood->setTexture("roughnessMap", woodRough);
		//wood->setTexture("aoMap", woodAo);
		//wood->setTexture("metallicMap", woodMetal);
		//wood->setTexture("emissionMap", default_emission);

		//Material* bricks = createResource<Material>(defaultShader);
		//bricks->setTexture("albedoMap", bricksAlbedo);
		//bricks->setTexture("normalMap", bricksNormal);
		//bricks->setTexture("roughnessMap", bricksRough);
		//bricks->setTexture("aoMap", bricksAo);
		//bricks->setTexture("metallicMap", bricksMetal);
		//bricks->setTexture("emissionMap", default_emission);

		//Material* gold = createResource<Material>(defaultShader);
		//gold->setTexture("albedoMap", goldAlbedo);
		//gold->setTexture("normalMap", goldNormal);
		//gold->setTexture("roughnessMap", goldRough);
		//gold->setTexture("aoMap", default_ao);
		//gold->setTexture("metallicMap", goldMetal);
		//gold->setTexture("emissionMap", default_emission);

		Material* rust = createResource<Material>(defaultShader);
		rust->setTexture("albedoMap", rustAlbedo);
		rust->setTexture("normalMap", rustNormal);
		rust->setTexture("roughnessMap", rustRough);
		rust->setTexture("aoMap", default_ao);
		rust->setTexture("metallicMap", rustMetal);
		rust->setTexture("emissionMap", default_emission);

		Material* def = createResource<Material>(defaultShader);
		def->setTexture("albedoMap", default_roughness);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_roughness);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);

		Material* chrome = createResource<Material>(defaultShader);
		chrome->setTexture("albedoMap", aluminium_a);
		chrome->setTexture("normalMap", aluminium_n);
		chrome->setTexture("roughnessMap", aluminium_r);
		chrome->setTexture("aoMap", default_ao);
		chrome->setTexture("metallicMap", aluminium_m);
		chrome->setTexture("emissionMap", default_emission);

		Material* paint = createResource<Material>(defaultShader);
		paint->setTexture("albedoMap", red);
		paint->setTexture("normalMap", default_normal);
		paint->setTexture("roughnessMap", default_emission);
		paint->setTexture("aoMap", default_ao);
		paint->setTexture("metallicMap", default_emission);
		paint->setTexture("emissionMap", default_emission);

		Material* rubber = createResource<Material>(defaultShader);
		rubber->setTexture("albedoMap", tire_text);
		rubber->setTexture("normalMap", default_normal);
		rubber->setTexture("roughnessMap", default_ao);
		rubber->setTexture("aoMap", default_ao);
		rubber->setTexture("metallicMap", default_emission);
		rubber->setTexture("emissionMap", default_emission);

		Material* glassMaterial = createResource<Material>(transparentShader);
		glassMaterial->setTexture("albedoMap", transparentAlbedo);
		glassMaterial->setTexture("normalMap", default_normal);
		glassMaterial->setTexture("roughnessMap", default_roughness);
		glassMaterial->setTexture("aoMap", default_ao);
		glassMaterial->setTexture("metallicMap", default_emission);
		glassMaterial->setTexture("emissionMap", default_emission);

		//Material* barkMaterial = createResource<Material>(defaultShader);
		//barkMaterial->setTexture("albedoMap", bark);
		//barkMaterial->setTexture("normalMap", default_normal);
		//barkMaterial->setTexture("roughnessMap", default_roughness);
		//barkMaterial->setTexture("aoMap", default_ao);
		//barkMaterial->setTexture("metallicMap", default_emission);
		//barkMaterial->setTexture("emissionMap", default_emission);

		//Material* leaveMaterial = createResource<Material>(defaultShader);
		//leaveMaterial->setTexture("albedoMap", leaveAlbedo);
		//leaveMaterial->setTexture("normalMap", leaveNormal);
		//leaveMaterial->setTexture("roughnessMap", leaveRoughness);
		//leaveMaterial->setTexture("aoMap", default_ao);
		//leaveMaterial->setTexture("metallicMap", default_emission);
		//leaveMaterial->setTexture("emissionMap", default_emission);

		Material* lit = createResource<Material>(litShader);
		lit->setProperty("color", Vector3f(0.3f, 0.3f, 0.5f));


		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<StateControllerScript>();
		cam->addComponent<CameraControllerScript>();


		/* --------------------------- LIGHTS --------------------------- */
		/*
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;
		*/

		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 10.0f;
		//sun->setEnable(false);

		
		Entity* spot = instanciate("Spot");
		spot->getComponent<Transform>()->setPosition(Vector3f(1, 7, -1));
		spot->getComponent<Transform>()->setRotation(Vector3f(-PI / 2.0f, 0.0f, -0.3f));
		spot->addComponent<SpotLight>();
		spot->getComponent<SpotLight>()->coneAngle = 0.97f;
		spot->getComponent<SpotLight>()->coneAttenuation = 0.96f;
		spot->getComponent<SpotLight>()->constant = 2.0f;
		spot->getComponent<SpotLight>()->linear = 0.09f;
		spot->getComponent<SpotLight>()->quadratic = 0.032f;
		spot->getComponent<SpotLight>()->color = Vector3f(0.97f, 0.96f, 0.98f);
		spot->getComponent<SpotLight>()->intensity = 3.6f;
		spot->setEnable(false);


		/*
		Entity* rig = instanciate("Rig");
		rig->addComponent<TransformerScript>();

		Entity* pointLight = instanciate("Pointlight");
		pointLight->getComponent<Transform>()->setPosition(Vector3f(-7, 2, 0));
		pointLight->addComponent<PointLight>();
		pointLight->getComponent<PointLight>()->color = Vector3f(215 / 255.0f, 30 / 255.0f, 45 / 255.0f);
		pointLight->getComponent<PointLight>()->constant = 2;
		pointLight->getComponent<PointLight>()->linear = 0.09f;
		pointLight->getComponent<PointLight>()->quadratic = 0.032f;
		pointLight->getComponent<PointLight>()->intensity = 1.6f;
		//pointLight->getComponent<Transform>()->setParent(rig);
		*/


		Entity* shelby_chassis_part = instanciate("shelby_chassis_part");
		shelby_chassis_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_chassis_part->addComponent<MeshRenderer>()->setMesh(shelby_chassis);
		shelby_chassis_part->getComponent<MeshRenderer>()->setMaterial(paint);

		Entity* shelby_chrome_part = instanciate("shelby_chrome_part");
		shelby_chrome_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_chrome_part->addComponent<MeshRenderer>()->setMesh(shelby_chrome);
		shelby_chrome_part->getComponent<MeshRenderer>()->setMaterial(chrome);

		Entity* shelby_tires_part = instanciate("shelby_tires_part");
		shelby_tires_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_tires_part->addComponent<MeshRenderer>()->setMesh(shelby_tires);
		shelby_tires_part->getComponent<MeshRenderer>()->setMaterial(rubber);

		
		Entity* floor = instanciate("Floor");
		floor->getComponent<Transform>()->setScale(Vector3f(30, 30, 30));
		floor->addComponent<MeshRenderer>()->setMesh(plane);
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

	void OnUnload() {}

};
#endif // PBR_SCENE_CPP