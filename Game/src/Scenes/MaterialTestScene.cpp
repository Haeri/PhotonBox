#ifndef MATERIAL_TEST_SCENE_CPP
#define MATERIAL_TEST_SCENE_CPP

#include <component/AmbientLight.h>
#include <component/Camera.h>
#include <component/MeshRenderer.h>
#include <component/MeshRenderer.h>
#include <component/PointLight.h>
#include <component/PointRenderer.h>
#include <component/LightProbe.h>
#include <resource/Scene.h>
#include <resource/shader/BasicShader.h>
#include <resource/shader/IrradianceShader.h>
#include <resource/shader/LitShader.h>
#include <resource/shader/TransparentShader.h>
#include <resource/shader/GShader.h>

#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/SSAOProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../PostProcessors/BlurProcessor.cpp"
#include "../PostProcessors/DOFProcessor.cpp"
#include "../PostProcessors/TAAProcessor.cpp"
#include "../Shader/BlurShader.cpp"
#include "../Scripts/StateControllerScript.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/MaterialScript.cpp"
#include "../Scripts/PrinterScript.cpp"
#include "../Scripts/TransformerScript.cpp"
#include "../Scripts/SpawnerScript.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class MaterialTestScene : public Scene
{
public:
	void Load()
	{
		/* --------------------------- SKYBOX --------------------------- */
		std::vector<std::string> forrest = {
			"./res/enviroment/Forrest/lod0_right.jpg",
			"./res/enviroment/Forrest/lod0_left.jpg",
			"./res/enviroment/Forrest/lod0_top.jpg",
			"./res/enviroment/Forrest/lod0_bottom.jpg",
			"./res/enviroment/Forrest/lod0_back.jpg",
			"./res/enviroment/Forrest/lod0_front.jpg",
		};

		Renderer::setSkyBox(createResource<CubeMap>(forrest));
		Renderer::getSkyBox()->intensity = 3;

		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor* p_ssao = new SSAOProcessor(10);
		SSReflectionProcessor* p_ssreflection = new SSReflectionProcessor(20);
		AutoExposureProcessor* p_autoExposure = new AutoExposureProcessor(30);
		TAAProcessor* p_fxaa = new TAAProcessor(40);
		DOFProcessor* p_dof = new DOFProcessor(50);
		BloomProcessor* p_bloom = new BloomProcessor(60);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(70);
		p_dof->setEnabled(false);
		
		/* --------------------------- OBJ --------------------------- */
		Mesh* plane = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/plane.obj");
		Mesh* sphere = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");

		/* --------------------------- TEXTURES --------------------------- */
		Texture* default_normal = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		Texture* default_roughness = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		Texture* transparentAlbedo = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/transparent.png"), true);

		Texture* woodAlbedo = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_basecolor.png"), true);
		Texture* woodRough = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_roughness.png"), true);
		Texture* woodNormal = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_normal.png"), true);
		Texture* woodAo = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_AO.png"), true);
		Texture* woodMetal = createResource<Texture>(std::string("./res/materials/mahogfloor/mahogfloor_metalness.png"), true);
		
		Texture* bricksAlbedo = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-albedo.png"), true);
		Texture* bricksRough = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-roughness.png"), true);
		Texture* bricksNormal = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-normal.png"), true);
		Texture* bricksAo = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-ao.png"), true);
		Texture* bricksMetal = createResource<Texture>(std::string("./res/materials/harshbricks/harshbricks-metalness.png"), true);
		
		Texture* rustAlbedo = createResource<Texture>(std::string("./res/materials/rust/rustediron2_basecolor.png"), true);
		Texture* rustRough = createResource<Texture>(std::string("./res/materials/rust/rustediron2_roughness.png"), true);
		Texture* rustNormal = createResource<Texture>(std::string("./res/materials/rust/rustediron2_normal.png"), true);
		Texture* rustMetal = createResource<Texture>(std::string("./res/materials/rust/rustediron2_metallic.png"), true);

		Texture* concrete_a = createResource<Texture>(std::string("./res/materials/concrete/concrete_a.jpg"), true);
		Texture* concrete_r = createResource<Texture>(std::string("./res/materials/concrete/concrete_r.jpg"), true);
		Texture* concrete_n = createResource<Texture>(std::string("./res/materials/concrete/concrete_n.jpg"), true);
		Texture* concrete_ao = createResource<Texture>(std::string("./res/materials/concrete/concrete_ao.jpg"), true);
		
		Texture* goldAlbedo = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-albedo.png"), true);
		Texture* goldRough = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-roughness.png"), true);
		Texture* goldNormal = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-normal.png"), true);
		Texture* goldMetal = createResource<Texture>(std::string("./res/materials/greasy-metal/greasy-metal-pan1-metal.png"), true);
		 

		/* --------------------------- SHADERS --------------------------- */
		TransparentShader* transparentShader = TransparentShader::getInstance();
		GShader* defaultShader = GShader::getInstance();

		/* --------------------------- MATERIALS --------------------------- */
		Material* wood = createResource<Material>(defaultShader);
		wood->setImageBuffer("albedoMap", woodAlbedo);
		wood->setImageBuffer("normalMap", woodNormal);
		wood->setImageBuffer("roughnessMap", woodRough);
		wood->setImageBuffer("aoMap", woodAo);
		wood->setImageBuffer("metallicMap", woodMetal);
		wood->setImageBuffer("emissionMap", default_emission);

		Material* bricks = createResource<Material>(defaultShader);
		bricks->setImageBuffer("albedoMap", bricksAlbedo);
		bricks->setImageBuffer("normalMap", bricksNormal);
		bricks->setImageBuffer("roughnessMap", bricksRough);
		bricks->setImageBuffer("aoMap", bricksAo);
		bricks->setImageBuffer("metallicMap", bricksMetal);
		bricks->setImageBuffer("emissionMap", default_emission);

		Material* gold = createResource<Material>(defaultShader);
		gold->setImageBuffer("albedoMap", goldAlbedo);
		gold->setImageBuffer("normalMap", goldNormal);
		gold->setImageBuffer("roughnessMap", goldRough);
		gold->setImageBuffer("aoMap", default_ao);
		gold->setImageBuffer("metallicMap", goldMetal);
		gold->setImageBuffer("emissionMap", default_emission);

		Material* chrome = createResource<Material>(defaultShader);
		chrome->setImageBuffer("albedoMap", default_ao);
		chrome->setImageBuffer("normalMap", default_normal);
		chrome->setImageBuffer("roughnessMap", default_emission);
		chrome->setImageBuffer("aoMap", default_ao);
		chrome->setImageBuffer("metallicMap", default_ao);
		chrome->setImageBuffer("emissionMap", default_emission);

		Material* rust = createResource<Material>(defaultShader);
		rust->setImageBuffer("albedoMap", rustAlbedo);
		rust->setImageBuffer("normalMap", rustNormal);
		rust->setImageBuffer("roughnessMap", rustRough);
		rust->setImageBuffer("aoMap", default_ao);
		rust->setImageBuffer("metallicMap", rustMetal);
		rust->setImageBuffer("emissionMap", default_emission);

		Material* glassMaterial = createResource<Material>(transparentShader);
		glassMaterial->setImageBuffer("albedoMap", transparentAlbedo);
		glassMaterial->setImageBuffer("normalMap", default_normal);
		glassMaterial->setImageBuffer("roughnessMap", default_roughness);
		glassMaterial->setImageBuffer("aoMap", default_ao);
		glassMaterial->setImageBuffer("metallicMap", default_emission);
		glassMaterial->setImageBuffer("emissionMap", default_emission);

		Material* def = createResource<Material>(defaultShader);
		def->setImageBuffer("albedoMap", default_roughness);
		def->setImageBuffer("normalMap", default_normal);
		def->setImageBuffer("roughnessMap", default_roughness);
		def->setImageBuffer("aoMap", default_ao);
		def->setImageBuffer("metallicMap", default_emission);
		def->setImageBuffer("emissionMap", default_emission);

		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<StateControllerScript>();
		cam->addComponent<CameraControllerScript>();
		cam->addComponent<SpawnerScript>();


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;
		
		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 10.0f;

		Entity* probe2 = instanciate("Probe-2");
		probe2->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		probe2->addComponent<MeshRenderer>();
		probe2->getComponent<MeshRenderer>()->setMesh(sphere);
		probe2->getComponent<MeshRenderer>()->setMaterial(chrome);

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
		probe6->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* probe7 = instanciate("Probe-7");
		probe7->getComponent<Transform>()->setPosition(Vector3f(30, 1, 0));
		probe7->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe7->getComponent<MeshRenderer>()->setMesh(sphere);
		probe7->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe8 = instanciate("Probe-7");
		probe8->getComponent<Transform>()->setPosition(Vector3f(36, 1, 0));
		probe8->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe8->getComponent<MeshRenderer>()->setMesh(sphere);
		probe8->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe9 = instanciate("Probe-7");
		probe9->getComponent<Transform>()->setPosition(Vector3f(42, 1, 0));
		probe9->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe9->getComponent<MeshRenderer>()->setMesh(sphere);
		probe9->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe10 = instanciate("Probe-7");
		probe10->getComponent<Transform>()->setPosition(Vector3f(48, 1, 0));
		probe10->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe10->getComponent<MeshRenderer>()->setMesh(sphere);
		probe10->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

	}

};
#endif // MATERIAL_TEST_SCENE_CPP