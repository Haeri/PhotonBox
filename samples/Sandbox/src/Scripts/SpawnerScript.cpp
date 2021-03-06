#ifndef SPAWNER_SCRIPT_CPP
#define SPAWNER_SCRIPT_CPP

#include <PhotonBox/component/Behaviour.h>
#include <PhotonBox/component/Transform.h>
#include <PhotonBox/component/MeshRenderer.h>
#include <PhotonBox/core/Entity.h>
#include <PhotonBox/core/Resources.h>
#include <PhotonBox/core/Time.h>
#include <PhotonBox/core/InputManager.h>
#include <PhotonBox/core/system/SceneManager.h>
#include <PhotonBox/resource/Scene.h>
#include <PhotonBox/resource/Texture.h>
#include <PhotonBox/resource/Mesh.h>
#include <PhotonBox/resource/Material.h>
#include <PhotonBox/resource/shader/GShader.h>

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

		_mesh = scene->createResource<Mesh>(Filepath("./res/meshes/fireplace_room.obj"));

		Texture* orig =				scene->createResource<Texture>(Filepath("./res/textures/view.png"));
		Texture* default_normal =	scene->createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_normal.png"));
		Texture* default_specular = scene->createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_specular.png"));
		Texture* default_emission = scene->createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_emission.png"));
		Texture* default_ao =		scene->createResource<Texture>(Filepath(Resources::ENGINE_RESOURCES + "/default_ao.png"));

		_mat = scene->createResource<Material>("defaultMaterial", GShader::getInstance());
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

		_night_c = scene->createResource<CubeMap>("night_cubemap", _nightSky);
		_studio_c = scene->createResource<CubeMap>("forrest_cubemap", _forrest);
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
		Entity* e = SceneManager::getCurrentScene()->instantiate("NewObject");
		e->getComponent<Transform>()->setPosition(Vector3f((float)(rand() % 10), (float)(rand() % 10), (float)(rand() % 10)));
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