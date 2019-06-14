#ifndef DEMO_SCENE_CPP
#define DEMO_SCENE_CPP

#include <chrono>

#include <component/MeshRenderer.h>
#include <component/PointRenderer.h>
#include <resource/Scene.h>
#include <resource/Texture.h>
#include <resource/shader/GShader.h>
#include <resource/shader/TransparentShader.h>
#include <core/Resources.h>
#include <math/Math.h>

#include "../PostProcessors/SSAOProcessor.cpp"
#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../PostProcessors/FXAAProcessor.cpp"
#include "../PostProcessors/DOFProcessor.cpp"
#include "../PostProcessors/TAAProcessor.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/StateControllerScript.cpp"
#include "../Scripts/SpawnerScript.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class ShelbyScene : public Scene
{
public:

	void Load()
	{

		/* --------------------------- SKYBOX --------------------------- */
		std::vector<std::string> studio = {
			"./res/enviroment/studio/face-l.jpg",
			"./res/enviroment/studio/face-r.jpg",
			"./res/enviroment/studio/face-t.jpg",
			"./res/enviroment/studio/face-b.jpg",
			"./res/enviroment/studio/face-d.jpg",
			"./res/enviroment/studio/face-f.jpg",
		};

		Renderer::setSkyBox(createResource<CubeMap>(studio));
		Renderer::getSkyBox()->intensity = 1;
		

		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor* p_ssao = new SSAOProcessor(10);
		SSReflectionProcessor* p_ssreflection = new SSReflectionProcessor(20);
		AutoExposureProcessor* p_autoExposure = new AutoExposureProcessor(30);
		TAAProcessor* p_fxaa = new TAAProcessor(40);
		//DOFProcessor* p_dof = new DOFProcessor(50);
		BloomProcessor* p_bloom = new BloomProcessor(60);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(70);
		//p_dof->setEnabled(false);


		/* --------------------------- OBJ --------------------------- */
		Mesh* plane = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/plane_big.obj");
		Mesh* shelby_chassis = createResource<Mesh>("./res/meshes/shelby/shelby_chassis.obj");
		Mesh* shelby_chrome = createResource<Mesh>("./res/meshes/shelby/shelby_chrome.obj");
		Mesh* shelby_tires = createResource<Mesh>("./res/meshes/shelby/shelby_tires.obj");
		Mesh* shelby_glass = createResource<Mesh>("./res/meshes/shelby/shelby_glass.obj");
		Mesh* shelby_front_lights = createResource<Mesh>("./res/meshes/shelby/shelby_front_lights.obj");
		Mesh* shelby_seats = createResource<Mesh>("./res/meshes/shelby/shelby_seats.obj");

		/* --------------------------- TEXTURES --------------------------- */
		Texture* default_normal = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		Texture* default_roughness = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		Texture* transparentAlbedo = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/transparent.png"), true);

		Texture* aluminium_a = createResource<Texture>(std::string("./res/materials/aluminium/albedo.png"), true);
		Texture* aluminium_r = createResource<Texture>(std::string("./res/materials/aluminium/roughness.png"), true);
		Texture* aluminium_n = createResource<Texture>(std::string("./res/materials/aluminium/normal.png"), true);
		Texture* aluminium_m = createResource<Texture>(std::string("./res/materials/aluminium/metallic.png"), true);

		Texture* leather_a = createResource<Texture>(std::string("./res/materials/leather/leather_a.png"), true);
		Texture* leather_r = createResource<Texture>(std::string("./res/materials/leather/leather_r.png"), true);
		Texture* leather_n = createResource<Texture>(std::string("./res/materials/leather/leather_n.png"), true);
		Texture* leather_ao = createResource<Texture>(std::string("./res/materials/leather/leather_ao.png"), true);

		Texture* tire_text = createResource<Texture>(std::string("./res/meshes/shelby/tire.png"), true);
		
		
		Texture* red = createResource<Texture>(std::string("./res/meshes/shelby/red.png"));

		Texture* light_grid = createResource<Texture>(std::string("./res/meshes/shelby/lights.png"), true);
		Texture* light_grid_r = createResource<Texture>(std::string("./res/meshes/shelby/lights_r.jpg"), true);


		/* --------------------------- SHADERS --------------------------- */
		GShader* defaultShader = GShader::getInstance();
		TransparentShader* transparentShader = TransparentShader::getInstance();

		/* --------------------------- MATERIALS --------------------------- */

		Material* chrome = createResource<Material>(defaultShader);
		chrome->setImageBuffer("albedoMap", aluminium_a);
		chrome->setImageBuffer("normalMap", aluminium_n);
		chrome->setImageBuffer("roughnessMap", aluminium_r);
		chrome->setImageBuffer("aoMap", default_ao);
		chrome->setImageBuffer("metallicMap", aluminium_m);
		chrome->setImageBuffer("emissionMap", default_emission);

		Material* paint = createResource<Material>(defaultShader);
		paint->setImageBuffer("albedoMap", red);
		paint->setImageBuffer("normalMap", default_normal);
		paint->setImageBuffer("roughnessMap", default_emission);
		paint->setImageBuffer("aoMap", default_ao);
		paint->setImageBuffer("metallicMap", default_emission);
		paint->setImageBuffer("emissionMap", default_emission);

		Material* rubber = createResource<Material>(defaultShader);
		rubber->setImageBuffer("albedoMap", tire_text);
		rubber->setImageBuffer("normalMap", default_normal);
		rubber->setImageBuffer("roughnessMap", default_ao);
		rubber->setImageBuffer("aoMap", default_ao);
		rubber->setImageBuffer("metallicMap", default_emission);
		rubber->setImageBuffer("emissionMap", default_emission);

		Material* leather = createResource<Material>(defaultShader);
		leather->setImageBuffer("albedoMap", leather_a);
		leather->setImageBuffer("normalMap", leather_n);
		leather->setImageBuffer("roughnessMap", leather_r);
		leather->setImageBuffer("aoMap", leather_ao);
		leather->setImageBuffer("metallicMap", default_emission);
		leather->setImageBuffer("emissionMap", default_emission);

		Material* front_emissive = createResource<Material>(defaultShader);
		front_emissive->setImageBuffer("albedoMap", light_grid);
		front_emissive->setImageBuffer("normalMap", default_normal);
		front_emissive->setImageBuffer("roughnessMap", light_grid_r);
		front_emissive->setImageBuffer("aoMap", default_ao);
		front_emissive->setImageBuffer("metallicMap", default_emission);
		front_emissive->setImageBuffer("emissionMap", light_grid);
		front_emissive->setProperty("emissionIntensity", 15.0f);

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
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -6));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraControllerScript>();
		cam->addComponent<StateControllerScript>();


		/* --------------------------- Entities --------------------------- */
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();
		ambient->getComponent<AmbientLight>()->color = Vector3f(0.17f, 0.17f, 0.17f);
		ambient->getComponent<AmbientLight>()->intensity = 0.4f;


		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 6.0f;

		Entity* floor = instanciate("floor");
		floor->getComponent<Transform>()->setScale(Vector3f(10.0f, 10.0f, 10.0f));
		floor->addComponent<MeshRenderer>()->setMesh(plane);
		floor->getComponent<MeshRenderer>()->setMaterial(def);

		Entity * shelby = instanciate("Shelby");

		Entity* shelby_chassis_part = instanciate("shelby_chassis_part");
		shelby_chassis_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_chassis_part->addComponent<MeshRenderer>()->setMesh(shelby_chassis);
		shelby_chassis_part->getComponent<MeshRenderer>()->setMaterial(paint);
		shelby_chassis_part->getComponent<MeshRenderer>()->getReflected = false;
		shelby_chassis_part->getComponent<Transform>()->setParent(shelby);


		Entity* shelby_chrome_part = instanciate("shelby_chrome_part");
		shelby_chrome_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_chrome_part->addComponent<MeshRenderer>()->setMesh(shelby_chrome);
		shelby_chrome_part->getComponent<MeshRenderer>()->setMaterial(chrome);
		shelby_chrome_part->getComponent<MeshRenderer>()->getReflected = false;
		shelby_chrome_part->getComponent<Transform>()->setParent(shelby);

		Entity* shelby_tires_part = instanciate("shelby_tires_part");
		shelby_tires_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_tires_part->addComponent<MeshRenderer>()->setMesh(shelby_tires);
		shelby_tires_part->getComponent<MeshRenderer>()->setMaterial(rubber);
		shelby_tires_part->getComponent<MeshRenderer>()->getReflected = false;
		shelby_tires_part->getComponent<Transform>()->setParent(shelby);

		Entity* shelby_front_lights_part = instanciate("shelby_front_lights");
		shelby_front_lights_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_front_lights_part->addComponent<MeshRenderer>()->setMesh(shelby_front_lights);
		shelby_front_lights_part->getComponent<MeshRenderer>()->setMaterial(front_emissive);
		shelby_front_lights_part->getComponent<MeshRenderer>()->getReflected = false;
		shelby_front_lights_part->getComponent<Transform>()->setParent(shelby);

		Entity* shelby_seats_part = instanciate("shelby_seats_part");
		shelby_seats_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_seats_part->addComponent<MeshRenderer>()->setMesh(shelby_seats);
		shelby_seats_part->getComponent<MeshRenderer>()->setMaterial(leather);
		shelby_seats_part->getComponent<MeshRenderer>()->getReflected = false;
		shelby_seats_part->getComponent<Transform>()->setParent(shelby);

		Entity* shelby_glass_part = instanciate("shelby_glass_part");
		shelby_glass_part->getComponent<Transform>()->setPosition(Vector3f(0, 0, 0));
		shelby_glass_part->getComponent<Transform>()->setParent(shelby);
		shelby_glass_part->addComponent<MeshRenderer>()->setRenderType(RenderType::RT_TRANSPARENT);
		shelby_glass_part->getComponent<MeshRenderer>()->setMesh(shelby_glass);
		shelby_glass_part->getComponent<MeshRenderer>()->setMaterial(glassMaterial);
		shelby_glass_part->getComponent<MeshRenderer>()->getReflected = false;


		Entity* spot = instanciate("Spot");
		spot->getComponent<Transform>()->setPosition(Vector3f(-1.0f, 1.0f, 1.0f));
		spot->getComponent<Transform>()->setRotation(Vector3f(0.0f, PI / 2.0f, 0.0f));
		spot->addComponent<SpotLight>();
		spot->getComponent<SpotLight>()->coneAngle = 0.97f;
		spot->getComponent<SpotLight>()->coneAttenuation = 0.96f;
		spot->getComponent<SpotLight>()->constant = 2.0f;
		spot->getComponent<SpotLight>()->linear = 0.09f;
		spot->getComponent<SpotLight>()->quadratic = 0.032f;
		spot->getComponent<SpotLight>()->color = Vector3f(0.97f, 0.96f, 0.98f);
		spot->getComponent<SpotLight>()->intensity = 100.0f;
		spot->getComponent<Transform>()->setParent(shelby);

		Entity* spot2 = instanciate("Spot2");
		spot2->getComponent<Transform>()->setPosition(Vector3f(-1.0f, 1.0f, -1.0f));
		spot2->getComponent<Transform>()->setRotation(Vector3f(0.0f, PI / 2.0f, 0.0f));
		spot2->addComponent<SpotLight>();
		spot2->getComponent<SpotLight>()->coneAngle = 0.97f;
		spot2->getComponent<SpotLight>()->coneAttenuation = 0.96f;
		spot2->getComponent<SpotLight>()->constant = 2.0f;
		spot2->getComponent<SpotLight>()->linear = 0.09f;
		spot2->getComponent<SpotLight>()->quadratic = 0.032f;
		spot2->getComponent<SpotLight>()->color = Vector3f(0.97f, 0.96f, 0.98f);
		spot2->getComponent<SpotLight>()->intensity = 100.0f;
		spot2->getComponent<Transform>()->setParent(shelby);
	}

	void OnUnload() {}

};
#endif // DEMO_SCENE_CPP