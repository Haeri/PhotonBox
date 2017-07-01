#include <iostream>
#include "../include/Core.h"

int main(void)
{

	// Create and initialize the RenderProject
	Core *project = new Core();
	project->init();
	delete project;


	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();

	return 0;
}