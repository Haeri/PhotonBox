#include <iostream>
#include <map>

#include <Core/Systems/Core.h>
//#include <Test/MathTest.h>
#include <Resources/Scene.h>

#include "Scenes/DemoScene.cpp"
#include "Scenes/PBRScene.cpp"
#include "Scenes/TestScene.cpp"
#include "Scenes/PhysicsScene.cpp"

int main(void)
{

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef _DEBUG
	// Tests
	//MathTest::startTest();
#else
	FreeConsole();
#endif

	// Create Scenes
	std::map<std::string, Scene*> sceneMap;
	sceneMap["Realistic Rendering"] = new TestScene();
	sceneMap["Material Test"] = new PBRScene();
	sceneMap["Demo Scene"] = new DemoScene();
	sceneMap["Physics Scene"] = new PhysicsScene();

	// Create and initialize the Core
	Core core;
	core.init(sceneMap, "Physics Scene");
	core.run();
	core.destroy();

#ifdef _DEBUG
	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();
#endif

	return 0;
}