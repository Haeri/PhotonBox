#include <iostream>
#include "Core/Systems/Core.h"
#include "Test\MathTest.h"

int main(void)
{

#ifdef _DEBUG
	// Tests
	MathTest::startTest();
#endif

	// Create and initialize the RenderProject
	Core core;
	core.init();
	core.run();
	core.destroy();

#ifdef _DEBUG
	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();
#endif

	return 0;
}