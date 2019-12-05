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

		Renderer::setSkyBox(createResource<CubeMap>("forrest_cubemap", forrest));
		Renderer::getSkyBox()->intensity = 1;

		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor* p_ssao					= new SSAOProcessor(10);
		SSReflectionProcessor* p_ssreflection	= new SSReflectionProcessor(20);
		AutoExposureProcessor* p_autoExposure	= new AutoExposureProcessor(30);
		TAAProcessor* p_fxaa					= new TAAProcessor(40);
		BloomProcessor* p_bloom					= new BloomProcessor(60);
		ToneMappingProcessor* p_tonemapping		= new ToneMappingProcessor(70);
		
		/* --------------------------- OBJ --------------------------- */
		Mesh* plane		= createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/plane.obj"));
		Mesh* sphere	= createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj"));

		/* --------------------------- TEXTURES --------------------------- */
		Texture::Config m_conf;
		m_conf.mips = true;

		Texture* default_normal		= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_normal.png"));
		Texture* default_specular	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_specular.png"));
		Texture* default_emission	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_emission.png"));
		Texture* default_ao			= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_ao.png"));
		Texture* default_roughness	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_roughness.png"));
		Texture* transparentAlbedo	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/transparent.png"), m_conf);

		Texture* woodAlbedo			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_basecolor.png"), m_conf);
		Texture* woodRough			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_roughness.png"), m_conf);
		Texture* woodNormal			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_normal.png"), m_conf);
		Texture* woodAo				= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_AO.png"), m_conf);
		Texture* woodMetal			= createResource<Texture>(Filepath("./res/materials/mahogfloor/mahogfloor_metalness.png"), m_conf);
		
		Texture* bricksAlbedo		= createResource<Texture>(Filepath("./res/materials/harshbricks/harshbricks-albedo.png"), m_conf);
		Texture* bricksRough		= createResource<Texture>(Filepath("./res/materials/harshbricks/harshbricks-roughness.png"), m_conf);
		Texture* bricksNormal		= createResource<Texture>(Filepath("./res/materials/harshbricks/harshbricks-normal.png"), m_conf);
		Texture* bricksAo			= createResource<Texture>(Filepath("./res/materials/harshbricks/harshbricks-ao.png"), m_conf);
		Texture* bricksMetal		= createResource<Texture>(Filepath("./res/materials/harshbricks/harshbricks-metalness.png"), m_conf);
		
		Texture* rustAlbedo			= createResource<Texture>(Filepath("./res/materials/rust/rustediron2_basecolor.png"), m_conf);
		Texture* rustRough			= createResource<Texture>(Filepath("./res/materials/rust/rustediron2_roughness.png"), m_conf);
		Texture* rustNormal			= createResource<Texture>(Filepath("./res/materials/rust/rustediron2_normal.png"), m_conf);
		Texture* rustMetal			= createResource<Texture>(Filepath("./res/materials/rust/rustediron2_metallic.png"), m_conf);

		Texture* concrete_a			= createResource<Texture>(Filepath("./res/materials/concrete/concrete_a.jpg"), m_conf);
		Texture* concrete_r			= createResource<Texture>(Filepath("./res/materials/concrete/concrete_r.jpg"), m_conf);
		Texture* concrete_n			= createResource<Texture>(Filepath("./res/materials/concrete/concrete_n.jpg"), m_conf);
		Texture* concrete_ao		= createResource<Texture>(Filepath("./res/materials/concrete/concrete_ao.jpg"), m_conf);
		
		Texture* goldAlbedo			= createResource<Texture>(Filepath("./res/materials/greasy-metal/greasy-metal-pan1-albedo.png"), m_conf);
		Texture* goldRough			= createResource<Texture>(Filepath("./res/materials/greasy-metal/greasy-metal-pan1-roughness.png"), m_conf);
		Texture* goldNormal			= createResource<Texture>(Filepath("./res/materials/greasy-metal/greasy-metal-pan1-normal.png"), m_conf);
		Texture* goldMetal			= createResource<Texture>(Filepath("./res/materials/greasy-metal/greasy-metal-pan1-metal.png"), m_conf);
		 

		/* --------------------------- SHADERS --------------------------- */
		TransparentShader* transparentShader = TransparentShader::getInstance();
		GShader* defaultShader = GShader::getInstance();

		/* --------------------------- MATERIALS --------------------------- */
		Material* wood = createResource<Material>("wood_material", defaultShader);
		wood->setImageBuffer("albedoMap", woodAlbedo);
		wood->setImageBuffer("normalMap", woodNormal);
		wood->setImageBuffer("roughnessMap", woodRough);
		wood->setImageBuffer("aoMap", woodAo);
		wood->setImageBuffer("metallicMap", woodMetal);
		wood->setImageBuffer("emissionMap", default_emission);

		Material* bricks = createResource<Material>("bricks_material", defaultShader);
		bricks->setImageBuffer("albedoMap", bricksAlbedo);
		bricks->setImageBuffer("normalMap", bricksNormal);
		bricks->setImageBuffer("roughnessMap", bricksRough);
		bricks->setImageBuffer("aoMap", bricksAo);
		bricks->setImageBuffer("metallicMap", bricksMetal);
		bricks->setImageBuffer("emissionMap", default_emission);

		Material* gold = createResource<Material>("gold_material", defaultShader);
		gold->setImageBuffer("albedoMap", goldAlbedo);
		gold->setImageBuffer("normalMap", goldNormal);
		gold->setImageBuffer("roughnessMap", goldRough);
		gold->setImageBuffer("aoMap", default_ao);
		gold->setImageBuffer("metallicMap", goldMetal);
		gold->setImageBuffer("emissionMap", default_emission);

		Material* chrome = createResource<Material>("chrome_material", defaultShader);
		chrome->setImageBuffer("albedoMap", default_ao);
		chrome->setImageBuffer("normalMap", default_normal);
		chrome->setImageBuffer("roughnessMap", default_emission);
		chrome->setImageBuffer("aoMap", default_ao);
		chrome->setImageBuffer("metallicMap", default_ao);
		chrome->setImageBuffer("emissionMap", default_emission);

		Material* rust = createResource<Material>("rust_material", defaultShader);
		rust->setImageBuffer("albedoMap", rustAlbedo);
		rust->setImageBuffer("normalMap", rustNormal);
		rust->setImageBuffer("roughnessMap", rustRough);
		rust->setImageBuffer("aoMap", default_ao);
		rust->setImageBuffer("metallicMap", rustMetal);
		rust->setImageBuffer("emissionMap", default_emission);

		Material* glassMaterial = createResource<Material>("glass_material", transparentShader);
		glassMaterial->setImageBuffer("albedoMap", transparentAlbedo);
		glassMaterial->setImageBuffer("normalMap", default_normal);
		glassMaterial->setImageBuffer("roughnessMap", default_roughness);
		glassMaterial->setImageBuffer("aoMap", default_ao);
		glassMaterial->setImageBuffer("metallicMap", default_emission);
		glassMaterial->setImageBuffer("emissionMap", default_emission);

		Material* def = createResource<Material>("def_material", defaultShader);
		def->setImageBuffer("albedoMap", default_roughness);
		def->setImageBuffer("normalMap", default_normal);
		def->setImageBuffer("roughnessMap", default_roughness);
		def->setImageBuffer("aoMap", default_ao);
		def->setImageBuffer("metallicMap", default_emission);
		def->setImageBuffer("emissionMap", default_emission);

		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instantiate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -10));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<StateControllerScript>();
		cam->addComponent<CameraControllerScript>();
		cam->addComponent<SpawnerScript>();


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instantiate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.3f, 0.3f, 0.3f);
		ambient->getComponent<AmbientLight>()->intensity = 0;
		
		Entity* sun = instantiate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(-1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 10.0f;

		Entity* probe2 = instantiate("Probe-2");
		probe2->getComponent<Transform>()->setPosition(Vector3f(0, 1, 0));
		probe2->addComponent<MeshRenderer>();
		probe2->getComponent<MeshRenderer>()->setMesh(sphere);
		probe2->getComponent<MeshRenderer>()->setMaterial(chrome);

		Entity* probe3 = instantiate("Probe-3");
		probe3->getComponent<Transform>()->setPosition(Vector3f(6, 1, 0));
		probe3->addComponent<MeshRenderer>();
		probe3->getComponent<MeshRenderer>()->setMesh(sphere);
		probe3->getComponent<MeshRenderer>()->setMaterial(bricks);

		Entity* probe4 = instantiate("Probe-4");
		probe4->getComponent<Transform>()->setPosition(Vector3f(12, 1, 0));
		probe4->addComponent<MeshRenderer>();
		probe4->getComponent<MeshRenderer>()->setMesh(sphere);
		probe4->getComponent<MeshRenderer>()->setMaterial(gold);

		Entity* probe5 = instantiate("Probe-5");
		probe5->getComponent<Transform>()->setPosition(Vector3f(18, 1, 0));
		probe5->addComponent<MeshRenderer>();
		probe5->getComponent<MeshRenderer>()->setMesh(sphere);
		probe5->getComponent<MeshRenderer>()->setMaterial(def);

		Entity* probe6 = instantiate("Probe-6");
		probe6->getComponent<Transform>()->setPosition(Vector3f(24, 1, 0));
		probe6->addComponent<MeshRenderer>();
		probe6->getComponent<MeshRenderer>()->setMesh(sphere);
		probe6->getComponent<MeshRenderer>()->setMaterial(wood);

		Entity* probe7 = instantiate("Probe-7");
		probe7->getComponent<Transform>()->setPosition(Vector3f(30, 1, 0));
		probe7->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe7->getComponent<MeshRenderer>()->setMesh(sphere);
		probe7->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe8 = instantiate("Probe-7");
		probe8->getComponent<Transform>()->setPosition(Vector3f(36, 1, 0));
		probe8->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe8->getComponent<MeshRenderer>()->setMesh(sphere);
		probe8->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe9 = instantiate("Probe-7");
		probe9->getComponent<Transform>()->setPosition(Vector3f(42, 1, 0));
		probe9->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe9->getComponent<MeshRenderer>()->setMesh(sphere);
		probe9->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

		Entity* probe10 = instantiate("Probe-7");
		probe10->getComponent<Transform>()->setPosition(Vector3f(48, 1, 0));
		probe10->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		probe10->getComponent<MeshRenderer>()->setMesh(sphere);
		probe10->getComponent<MeshRenderer>()->setMaterial(glassMaterial);

	}

};
#endif // MATERIAL_TEST_SCENE_CPP