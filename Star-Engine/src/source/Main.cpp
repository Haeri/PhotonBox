#include <iostream>
#include "../header/Core.h"

int main(void)
{

	// Create and initialize the RenderProject
	Core core;
	core.init();
	core.update();
	core.destroy();

	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();

	return 0;
}