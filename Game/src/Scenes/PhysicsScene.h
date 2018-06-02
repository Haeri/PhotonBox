#ifndef PHYSICS_SCENE_H
#define PHYSICS_SCENE_H

#include <Components/MeshRenderer.h>
#include <Components/Rigidbody.h>
#include <Resources/OBJLoader.h>
#include <Resources/Scene.h>
#include <Resources/Texture.h>
#include <Components/SphereCollider.h>
#include <Components/BoxCollider.h>

#include "../PostProcessors/ToneMappingProcessor.cpp"
#include "../PostProcessors/AutoExposureProcessor.cpp"
#include "../Scripts/CameraControllerScript.cpp"

class PhysicsScene : public Scene
{
public:
	CubeMap * sky;

	Mesh* planeMesh;
	Mesh* sphereMesh;
	Mesh* boxMesh;

	Texture* default_normal;
	Texture* default_specular;
	Texture* default_emission;
	Texture* default_ao;
	Texture* gradient;
	Texture* default_roughness;

	Material* def;

	AutoExposureProcessor* p_autoExposure;
	ToneMappingProcessor* p_tonemapping;

	void Load()
	{

		/* --------------------------- RESOURCES --------------------------- */
		std::vector<std::string> nightSky = {
			Resources::ENGINE_RESOURCES + "/default_specular.png",
			Resources::ENGINE_RESOURCES + "/default_specular.png",
			Resources::ENGINE_RESOURCES + "/default_specular.png",
			Resources::ENGINE_RESOURCES + "/default_specular.png",
			Resources::ENGINE_RESOURCES + "/default_specular.png",
			Resources::ENGINE_RESOURCES + "/default_specular.png",
		};

		sky = new CubeMap(nightSky);
		Renderer::setSkyBox(sky);
		Renderer::getSkyBox()->intensity = 1;


		/* --------------------------- POST PROCESSING --------------------------- */
		p_autoExposure = new AutoExposureProcessor(1);
		p_tonemapping = new ToneMappingProcessor(3);


		/* --------------------------- OBJ --------------------------- */
		planeMesh = OBJLoader::loadObj(Resources::ENGINE_RESOURCES + "/primitives/plane.obj");
		sphereMesh = OBJLoader::loadObj(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");
		boxMesh = OBJLoader::loadObj(Resources::ENGINE_RESOURCES + "/primitives/cube.obj");


		/* --------------------------- TEXTURES --------------------------- */
		default_normal = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		default_specular = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		default_emission = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		default_ao = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);
		default_roughness = new Texture(std::string(Resources::ENGINE_RESOURCES + "/default_roughness.png"), false);
		gradient = new Texture(std::string(Resources::ENGINE_RESOURCES + "/gradient.png"), false);

		/* --------------------------- SHADERS --------------------------- */



		/* --------------------------- MATERIALS --------------------------- */
		def = new Material();
		def->setTexture("albedoMap", default_ao);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_specular);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);


		/* --------------------------- CAMERA --------------------------- */
		Entity* cam = instanciate("Camera");
		cam->addComponent<Camera>();
		cam->getComponent<Transform>()->setPosition(Vector3f(0, 1, -6));
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
		sun->getComponent<DirectionalLight>()->intensity = 10.0f;	


		for (size_t i = 0; i < 1; i++)
		{
			for (size_t j = 0; j < 1; j++)
			{
				for (size_t z = 0; z < 1; z++)
				{
			
		Entity* sphere = instanciate("Sphere" + i);
		sphere->getComponent<Transform>()->setPosition(Vector3f(0.8, 5, 0));
		//sphere->getComponent<Transform>()->setPosition(Vector3f(i * 3 + (rand() % 2), 6 + z * 3 + (rand() % 2), j * 3 + (rand() % 2)));
		sphere->addComponent<MeshRenderer>()->setMesh(sphereMesh);
		sphere->getComponent<MeshRenderer>()->setMaterial(def);
		sphere->addComponent<Rigidbody>();
		sphere->addComponent<SphereCollider>()->setRadius(1);
		
		Entity* box = instanciate("Box" + std::to_string(i));
		box->getComponent<Transform>()->setPosition(Vector3f(0, 9, 0));
		//box->getComponent<Transform>()->setPosition(Vector3f(i * 3 + (rand() % 2), 6 + z * 3 + (rand() % 2), j * 3 + (rand() % 2)));
		//box->getComponent<Transform>()->setRotation(Vector3f(rand() % 4, 0, 0));
		box->addComponent<MeshRenderer>()->setMesh(boxMesh);
		box->getComponent<MeshRenderer>()->setMaterial(def);
		box->addComponent<Rigidbody>();
		box->addComponent<BoxCollider>()->setHalfExtents(Vector3f(1));
					

					//std::cout << std::to_string(i + j + z) << std::endl;
				}
			}
		}

		


		Entity* quad = instanciate("Plane");
		quad->getComponent<Transform>()->setPosition(Vector3f(0, 0, -3));
		quad->getComponent<Transform>()->setScale(Vector3f(20, 20, 20));
		quad->addComponent<MeshRenderer>();
		quad->getComponent<MeshRenderer>()->setMesh(planeMesh);
		quad->getComponent<MeshRenderer>()->setMaterial(def);
	}

	void OnUnload()
	{
		delete sky;

		delete planeMesh;
		delete sphereMesh;
		delete boxMesh;

		delete default_normal;
		delete default_specular;
		delete default_emission;
		delete default_ao;
		delete gradient;
		delete default_roughness;

		delete def;
	}

};
#endif // PHYSICS_SCENE_H