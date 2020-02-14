#include <map>

#include <core/Core.h>
#include <resource/Scene.h>

#include "Scenes/ShelbyScene.cpp"
#include "Scenes/MaterialTestScene.cpp"
#include "Scenes/PhysicsScene.cpp"
#include "Scenes/RealisticRenderingScene.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

extern "C"
{
	__declspec(dllexport) void getSceneMap(std::map<std::string, Scene*> &sceneMap)
	{
		// Create Scenes
		sceneMap["1. Material Test"] = new MaterialTestScene();
		sceneMap["2. Shelby Scene"] = new ShelbyScene();
		sceneMap["3. Physics Scene"] = new PhysicsScene();
		sceneMap["4. Realistic Rendering"] = new RealisticRenderingScene();
	}
}