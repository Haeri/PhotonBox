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

		Renderer::setSkyBox(createResource<CubeMap>("night_cubemap", nightSky));
		Renderer::getSkyBox()->intensity = 1;


		/* --------------------------- POST PROCESSING --------------------------- */
		new SSAOProcessor(0);
		new SSReflectionProcessor(10);
		new AutoExposureProcessor(20);
		new BloomProcessor(30);
		new ToneMappingProcessor(40);


		/* --------------------------- OBJ --------------------------- */
		Mesh* planeMesh		= createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/plane.obj"));
		Mesh* sphereMesh	= createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj"));
		Mesh* boxMesh		= createResource<Mesh>(Filepath(Resources::ENGINE_RESOURCES + "/primitives/cube.obj"));


		/* --------------------------- TEXTURES --------------------------- */
		Texture* default_normal		= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_normal.png"));
		Texture* default_specular	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_specular.png"));
		Texture* default_emission	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_emission.png"));
		Texture* default_ao			= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_ao.png"));
		Texture* default_roughness	= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_roughness.png"));
		Texture* gradient			= createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/gradient.png"));

		/* --------------------------- SHADERS --------------------------- */
		GShader* defaultShader = GShader::getInstance();


		/* --------------------------- MATERIALS --------------------------- */
		Material* def = createResource<Material>("def_material", defaultShader);
		def->setImageBuffer("albedoMap", default_ao);
		def->setImageBuffer("normalMap", default_normal);
		def->setImageBuffer("roughnessMap", default_specular);
		def->setImageBuffer("aoMap", default_ao);
		def->setImageBuffer("metallicMap", default_emission);
		def->setImageBuffer("emissionMap", default_emission);


		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instantiate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 6, -30));
		cam->getComponent<Transform>()->setRotation(Vector3f(0, 0, 0));
		cam->addComponent<CameraControllerScript>();
		cam->addComponent<StateControllerScript>();


		/* --------------------------- LIGHTS --------------------------- */
		Entity* ambient = instantiate("Ambient");
		ambient->addComponent<AmbientLight>();

		Entity* sun = instantiate("Sun");
		sun->addComponent<DirectionalLight>();
		sun->getComponent<DirectionalLight>()->color = Vector3f(0.93f, 0.92f, 0.94f);
		sun->getComponent<DirectionalLight>()->direction = Vector3f(1, -1, 1);
		sun->getComponent<DirectionalLight>()->intensity = 40.0f;			

		
		for (size_t i = 0; i < 100; i++)
		{
			float scale = rand() % 100 * 0.04f + 1;
			//float scale = 1.5f;
			Entity* sphere = instantiate("Sphere" + std::to_string(i));
			sphere->getComponent<Transform>()->setPosition(Vector3f((float)(rand() % 20 - 10), (float)(rand() % 20 + 10), (float)(rand() % 20-10)));
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
					Entity* box = instantiate("Box" + std::to_string(i) + std::to_string(j) + std::to_string(z));
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
		
		Entity* quad = instantiate("Plane");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(200, 200, 200));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(planeMesh);
		quad->getComponent<MeshRenderer>()->setMaterial(def);
		
	}

	void OnUnload() {}

};
#endif // PHYSICS_SCENE_CPP