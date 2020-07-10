#include <iostream>
#include <map>
#include <filesystem>

#include <PhotonBox/core/Core.h>
#include <PhotonBox/resource/Scene.h>

#include "Scenes/ShelbyScene.cpp"
#include "Scenes/MaterialTestScene.cpp"
#include "Scenes/PhysicsScene.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

int main(void)
{
#ifdef PB_MEM_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#elif _DEBUG
#else
#ifdef _WINDOWS
	FreeConsole();
#endif
#endif

	std::filesystem::path cwd = std::filesystem::current_path();
	std::cout << cwd << std::endl;

	// Create Scenes
	std::map<std::string, Scene*> sceneMap;
	sceneMap["1. Material Test"] = new MaterialTestScene();
	sceneMap["2. Shelby Scene"] = new ShelbyScene();
	sceneMap["3. Physics Scene"] = new PhysicsScene();

	// Create and initialize the Core
	Core core;
	core.init(sceneMap);
	core.run();
	core.destroy();

#ifdef _DEBUG
	std::cin.ignore();
#endif

	return 0;
}