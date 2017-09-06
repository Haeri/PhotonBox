#include <iostream>
#include "Core/Systems/Core.h"
#include "Test\MathTest.h"

int main(void)
{

	// Tests
	MathTest::startTest();

	// Create and initialize the RenderProject
	Core core;
	core.init();
	core.update();
	core.destroy();

	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();

	return 0;
}
