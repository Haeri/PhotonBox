#ifndef SPAWNER_SCRIPT_CPP
#define SPAWNER_SCRIPT_CPP

#include <component/Behaviour.h>
#include <component/Transform.h>
#include <component/MeshRenderer.h>
#include <core/Entity.h>
#include <core/Resources.h>
#include <core/Time.h>
#include <core/InputManager.h>
#include <core/system/SceneManager.h>
#include <resource/Scene.h>
#include <resource/Texture.h>
#include <resource/Mesh.h>
#include <resource/Material.h>
#include <resource/shader/GShader.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class SpawnerScript : public Behaviour
{
public:
	float speed = 1;

	void Update()
	{
		if (InputManager::keyPressed(InputManager::KEY_R)) {
			createEntity();
		}
	}

	void createEntity() 
	{
		Scene* scene = SceneManager::getCurrentScene();
		//Mesh* sphere = scene->createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");
		Mesh* sphere = scene->createResource<Mesh>("./res/meshes/fireplace_room.obj");

		Texture* orig = scene->createResource<Texture>(std::string("./res/textures/view.png"), false);
		Texture* default_normal = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);

		GShader* defaultShader = GShader::getInstance();

		Material* def = scene->createResource<Material>(defaultShader);
		def->setTexture("albedoMap", orig);
		def->setTexture("normalMap", default_normal);
		def->setTexture("roughnessMap", default_specular);
		def->setTexture("aoMap", default_ao);
		def->setTexture("metallicMap", default_emission);
		def->setTexture("emissionMap", default_emission);

		Entity* e = scene->instanciate("NewObject");
		e->getComponent<Transform>()->setPosition(Vector3f(rand() % 10, rand() % 10, rand() % 10));
		e->addComponent<MeshRenderer>();
		e->getComponent<MeshRenderer>()->setMesh(sphere);
		e->getComponent<MeshRenderer>()->setMaterial(def);
	}
};

#endif // SPAWNER_SCRIPT_CPP