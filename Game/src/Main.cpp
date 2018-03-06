#include <iostream>
#include <map>
#include "Core/Systems/Core.h"
#include "Test/MathTest.h"
#include "Resources/Scene.h"
#include "Game/DemoScene.h"
#include "Game/PBRScene.h"
#include "Game/TestScene.h"

int main(void)
{

#ifdef _DEBUG
	// Tests
	MathTest::startTest();
#endif

	// Create Scenes
	std::map<std::string, Scene*> sceneMap;
	sceneMap["Realistic Rendering"] = new TestScene();
	sceneMap["Material Test"] = new PBRScene();
	sceneMap["Demo Scene"] = new DemoScene();

	// Create and initialize the RenderProject
	Core core;
	core.init(sceneMap);
	core.run();
	core.destroy();

#ifdef _DEBUG
	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();
#endif

	return 0;
}
