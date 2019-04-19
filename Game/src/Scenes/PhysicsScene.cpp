#ifndef PHYSICS_SCENE_CPP
#define PHYSICS_SCENE_CPP

#include <component/MeshRenderer.h>
#include <component/Rigidbody.h>
#include <component/SphereCollider.h>
#include <component/BoxCollider.h>
#include <resource/Scene.h>
#include <resource/Texture.h>
#include <resource/shader/GShader.h>

#include "../PostProcessors/SSAOProcessor.cpp"
#include "../PostProcessors/SSReflectionProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../PostProcessors/BloomProcessor.cpp"
#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../Scripts/CameraControllerScript.cpp"
#include "../Scripts/StateControllerScript.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class PhysicsScene : public Scene
{
public:

	void Load()
	{
		/* --------------------------- RESOURCES --------------------------- */
		std::vector<std::string> nightSky = {
			Resources::ENGINE_RESOURCES + "/default_roughness.png",
			Resources::ENGINE_RESOURCES + "/default_roughness.png",
			Resources::ENGINE_RESOURCES + "/default_roughness.png",
			Resources::ENGINE_RESOURCES + "/default_roughness.png",
			Resources::ENGINE_RESOURCES + "/default_roughness.png",
			Resources::ENGINE_RESOURCES + "/default_roughness.png",
		};

		Renderer::setSkyBox(createResource<CubeMap>(nightSky));
		Renderer::getSkyBox()->intensity = 1;


		/* --------------------------- POST PROCESSING --------------------------- */
		SSAOProcessor * p_ssao = new SSAOProcessor(0);
		SSReflectionProcessor* p_ssreflection = new SSReflectionProcessor(1);
		AutoExposureProcessor* p_autoExposure = new AutoExposureProcessor(2);
		BloomProcessor* p_bloom = new BloomProcessor(3);
		ToneMappingProcessor* p_tonemapping = new ToneMappingProcessor(4);


		/* --------------------------- OBJ --------------------------- */
		Mesh* planeMesh = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/plane.obj");
		Mesh* sphereMesh = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");
		Mesh* boxMesh = createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/cube.obj");


		/* --------------------------- TEXTURES --------------------------- */
		Texture* default_normal = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		Texture* default_roughness = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		Texture* gradient = createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/gradient.png"), false);

		/* --------------------------- SHADERS --------------------------- */
		GShader* defaultShader = GShader::getInstance();



		/* --------------------------- MATERIALS --------------------------- */
		Material* def = createResource<Material>(defaultShader);
		def->setTexture("albedoMap", default_ao);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_specular);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);


		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 6, -30));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraControllerScript>();
		cam->addComponent<StateControllerScript>();


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instanciate("Ambient");
		ambient->addComponent<AmbientLight>();

		Entity* sun = instanciate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 40.0f;			

		
		for (size_t i = 0; i < 100; i++)
		{
			float scale = rand() % 100 * 0.04f + 1;
			//float scale = 1.5f;
			std::cout << scale << std::endl;
			Entity* sphere = instanciate("Sphere" + std::to_string(i));
			sphere->getComponent<Transform>()->setPosition(Vector3f(rand() % 20 - 10, rand() % 20 + 10, rand() % 20-10));
			//sphere->getComponent<Transform>()->setScale(Vector3f(scale, scale, scale));
			sphere->addComponent<MeshRenderer>()->setMesh(sphereMesh);
			sphere->getComponent<MeshRenderer>()->setMaterial(def);
			sphere->addComponent<SphereCollider>()->setRadius(1);
			sphere->addComponent<Rigidbody>()->setMass(scale);
		}
		

		
		/*
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t z = 0; z < 4; z++)
				{
					Entity* box = instanciate("Box" + std::to_string(i) + std::to_string(j) + std::to_string(z));
					//box->getComponent<Transform>()->setPosition(Vector3f(0, 9, 0));
					box->getComponent<Transform>()->setPosition(Vector3f(i * 3 + (rand() % 2), 6 + z * 3 + (rand() % 2), j * 3 + (rand() % 2)));
					box->addComponent<MeshRenderer>()->setMesh(boxMesh);
					box->getComponent<MeshRenderer>()->setMaterial(def);
					box->addComponent<Rigidbody>();
					//box->addComponent<PointLight>()->color = Vector3f((i/4.0f * 255.0f), (j / 4.0f * 255.0f), (z / 4.0f * 255.0f));
					box->addComponent<BoxCollider>()->setHalfExtents(Vector3f(1));
				}
			}
		}
		*/
		
		Entity* quad = instanciate("Plane");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(200, 200, 200));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(planeMesh);
		quad->getComponent<MeshRenderer>()->setMaterial(def);
		
	}

	void OnUnload() {}

};
#endif // PHYSICS_SCENE_CPP