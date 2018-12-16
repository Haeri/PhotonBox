#include <iostream>
#include <map>

#include <Core/Systems/Core.h>
//#include <Test/MathTest.h>
#include <Resources/Scene.h>

#include "Scenes/DemoScene.cpp"
#include "Scenes/PBRScene.cpp"
#include "Scenes/TestScene.cpp"
#include "Scenes/PhysicsScene.cpp"


#include <PhotonBox/core/Util.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

int main(void)
{
	/*
	std::cout << "---------- Starting ---------- " << std::endl;

	std::cout << sizeof(int) << std::endl;

	int i = 12;
	float f = 92.345f;
	int* re_i;
	float* re_f;

	unsigned char* buff = Util::combineBuffer(4, (unsigned char*)&i, sizeof(i), (unsigned char*)&f, sizeof(float));
	unsigned char* curr = buff;

	re_i = reinterpret_cast<int*>(curr);
	curr += sizeof(int);
	re_f = reinterpret_cast<float*>(curr);

	std::cout << "Initial Values: \n";
	std::cout << "Int: " << i << "\n";
	std::cout << "Flt: " << f << "\n";
	
	std::cout << "\nAfter Values: \n";
	std::cout << "Int: " << *re_i << "\n";
	std::cout << "Flt: " << *re_f << "\n";
	
	free(buff);
	std::cout << "---------- Ending ---------- " << std::endl;
	*/

	/*
	size_t cp_size;
	const size_t ucp_size = 50;
	char a[ucp_size] = "Hello Hello Hello Hello Hello Hello!";
	char* b;

	unsigned char* cp_buff = Util::compress((unsigned char*)a, ucp_size, &cp_size);
	b = (char*)Util::decompress(cp_buff, cp_size, ucp_size);

	printf("Uncompressed size is: %lu\n", strlen(a));
	printf("Uncompressed string is: %s\n", a);

	// This is one way of getting the size of the output
	printf("Compressed size is: %lu\n", strlen(b));
	printf("Compressed string is: %s\n", b);



	free(cp_buff);
	free(b);
	*/

	//std::cout << sizeof(size_t) << " " << sizeof(short) << std::endl;

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
	core.init(sceneMap, "Demo Scene");
	core.run();
	core.destroy();

#ifdef _DEBUG
	std::cout << "press ENTER to quit" << std::endl;
	std::cin.ignore();
#endif

	return 0;
}