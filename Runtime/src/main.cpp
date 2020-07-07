#include <iostream>
#include <map>
#include <windows.h>

#include <PhotonBox/core/Core.h>
#include <PhotonBox/resource/Scene.h>
#include <PhotonBox/util/Logger.h>

#include "../test/MathTest.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

typedef void (*GAMEMODULE)(std::map<std::string, Scene*>& sceneMap);

int main(void)
{
#ifdef PB_MEM_DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#elif _DEBUG
	// Tests
	//MathTest::startTest();
#else
#ifdef PB_PLATFORM_WIN
	FreeConsole();
#endif
#endif

	// Create Scenes
	std::map<std::string, Scene*> sceneMap;


	HINSTANCE hinstLib;
	GAMEMODULE ProcAdd;
	bool fFreeResult = false;

	hinstLib = LoadLibrary(TEXT("Game.dll"));

	if (hinstLib != NULL)
	{
		ProcAdd = (GAMEMODULE)GetProcAddress(hinstLib, "getSceneMap");

		if (NULL != ProcAdd)
		{
		
			ProcAdd(sceneMap);

			// Create and initialize the Core
			Core core;
			core.init(sceneMap);
			core.run();
			core.destroy();
		}
		else 
		{
			Logger::err("Coul not load Game functions!");
		}
	}
	else
	{
		Logger::err("Could not load the Game module!");
	}

	fFreeResult = FreeLibrary(hinstLib);

#ifdef _DEBUG
	std::cin.ignore();
#endif

	return 0;
}