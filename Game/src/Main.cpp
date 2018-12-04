#include <iostream>
#include <map>

#include <Core/Systems/Core.h>
//#include <Test/MathTest.h>
#include <Resources/Scene.h>

#include "Scenes/DemoScene.cpp"
#include "Scenes/PBRScene.cpp"
#include "Scenes/TestScene.cpp"
#include "Scenes/PhysicsScene.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

int main(void)
{

#ifdef MEM_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#elif _DEBUG
	// Tests
	//MathTest::startTest();
#else
	FreeConsole();
#endif

	// Create Scenes
	std::map<std::string, Scene*> sceneMap;
	sceneMap["Demo Scene"] = new DemoScene();
	sceneMap["Realistic Rendering"] = new TestScene();
	sceneMap["Material Test"] = new PBRScene();
	sceneMap["Physics Scene"] = new PhysicsScene();
	

	// Create and initialize the Core
	Core core;
	core.init(sceneMap, "Realistic Rendering");
	core.run();
	core.destroy();

#ifdef _DEBUG
	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();
#endif

	return 0;
}