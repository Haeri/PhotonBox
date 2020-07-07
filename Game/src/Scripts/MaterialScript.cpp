#ifndef MATERIAL_SCRIPT_CPP
#define MATERIAL_SCRIPT_CPP

#include <PhotonBox/component/Behaviour.h>
#include <PhotonBox/component/Transform.h>
#include <PhotonBox/core/InputManager.h>
#include <PhotonBox/resource/Material.h>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class MaterialScript : public Behaviour
{
public:
	Material * material;

	void Update()
	{
		if (InputManager::keyPressed(InputManager::KEY_UP))
		{
			float s = material->getProperty<float>("exposure");
			material->setProperty<float>("exposure", s += 0.01f);
		}

		if (InputManager::keyPressed(InputManager::KEY_DOWN))
		{
			float s = material->getProperty<float>("exposure");
			material->setProperty<float>("exposure", s -= 0.01f);
		}
	}
};

#endif // CAMERA_CONTROLLER_CPP