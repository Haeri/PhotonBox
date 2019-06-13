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

	void Start() override
	{
		Scene* scene = SceneManager::getCurrentScene();

		_mesh = scene->createResource<Mesh>("./res/meshes/fireplace_room.obj");

		Texture* orig = scene->createResource<Texture>(std::string("./res/textures/view.png"), false);
		Texture* default_normal = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_normal.png"), false);
		Texture* default_specular = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_specular.png"), false);
		Texture* default_emission = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_emission.png"), false);
		Texture* default_ao = scene->createResource<Texture>(std::string(Resources::ENGINE_RESOURCES + "/default_ao.png"), false);

		_mat = scene->createResource<Material>(GShader::getInstance());
		_mat->setImageBuffer("albedoMap", orig);
		_mat->setImageBuffer("normalMap", default_normal);
		_mat->setImageBuffer("roughnessMap", default_specular);
		_mat->setImageBuffer("aoMap", default_ao);
		_mat->setImageBuffer("metallicMap", default_emission);
		_mat->setImageBuffer("emissionMap", default_emission);

		_nightSky = {
			"./res/enviroment/dark/posx.jpg",
			"./res/enviroment/dark/negx.jpg",
			"./res/enviroment/dark/posy.jpg",
			"./res/enviroment/dark/negy.jpg",
			"./res/enviroment/dark/posz.jpg",
			"./res/enviroment/dark/negz.jpg",
		};
		
		_forrest = {
			"./res/enviroment/Forrest/lod0_right.jpg",
			"./res/enviroment/Forrest/lod0_left.jpg",
			"./res/enviroment/Forrest/lod0_top.jpg",
			"./res/enviroment/Forrest/lod0_bottom.jpg",
			"./res/enviroment/Forrest/lod0_back.jpg",
			"./res/enviroment/Forrest/lod0_front.jpg",
		};

		_night_c = scene->createResource<CubeMap>(_nightSky);
		_studio_c = scene->createResource<CubeMap>(_forrest);
	}

	void Update()
	{
		if (InputManager::keyPressed(InputManager::KEY_R)) {
			createEntity();
		}

		if (InputManager::keyPressed(InputManager::KEY_K)) {
			toggleSkybox();
		}
	}

	void toggleSkybox()
	{
		if (_sky_flip)
		{
			Renderer::setSkyBox(_studio_c);
		}
		else
		{
			Renderer::setSkyBox(_night_c);
		}
		_sky_flip = !_sky_flip;
	}

	void createEntity() 
	{
		Entity* e = SceneManager::getCurrentScene()->instanciate("NewObject");
		e->getComponent<Transform>()->setPosition(Vector3f(rand() % 10, rand() % 10, rand() % 10));
		e->addComponent<MeshRenderer>();
		e->getComponent<MeshRenderer>()->setMesh(_mesh);
		e->getComponent<MeshRenderer>()->setMaterial(_mat);
	}

private:
	std::vector<std::string> _nightSky;
	std::vector<std::string> _forrest;

	CubeMap* _night_c;
	CubeMap* _studio_c;

	Material* _mat;
	Mesh* _mesh;

	bool _sky_flip;
};

#endif // SPAWNER_SCRIPT_CPP