#ifndef SPAWNER_SCRIPT_CPP
#define SPAWNER_SCRIPT_CPP

#include <Components/Behaviour.h>
#include <Components/Transform.h>
#include <Core/Time.h>
#include <core/InputManager.h>
#include <core/systems/SceneManager.h>
#include <resources/Scene.h>
#include <resources/Texture.h>
#include <resources/Mesh.h>
#include <resources/Resources.h>
#include <resources/GShader.h>
#include <resources/Material.h>
#include <components/MeshRenderer.h>
#include <core/Entity.h>

#ifdef MEM_DEBUG
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
		Mesh* sphere = scene->createResource<Mesh>(Resources::ENGINE_RESOURCES + "/primitives/sphere.obj");

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