#include "PhotonBox/core/Config.h"

#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>

#include "PhotonBox/util/Util.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Config::Profile Config::profile;

void Config::readConfig()
{
	std::ifstream file(FILE_PATH);
	std::string line;
	std::vector<std::string> tokens;

	if (file.fail())
	{
		Logger::infoln("Creating new config file.");
		createDefault();

		return;
	}

	while (std::getline(file, line))
	{
		tokens.clear();
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		Util::split(line, "=", tokens);

		if (tokens[0] == "" || tokens[0].at(0) == '#')
		{
			continue;
		}
		else if (tokens[0].compare("appName") == 0)
		{
			profile.appName = tokens[1];
		}
		else if (tokens[0].compare("width") == 0)
		{
			profile.width = std::stoi(tokens[1]);
		}
		else if (tokens[0].compare("height") == 0)
		{
			profile.height = std::stoi(tokens[1]);
		}
		else if (tokens[0].compare("fullscreen") == 0)
		{
			profile.fullscreen = (bool)std::stoi(tokens[1]);
		}
		else if (tokens[0].compare("vsync") == 0)
		{
			profile.vsync = (bool)std::stoi(tokens[1]);
		}
		else if (tokens[0].compare("supersampling") == 0)
		{
			profile.supersampling = (float)std::stof(tokens[1]);
		}
		else if (tokens[0].compare("show_fps") == 0)
		{
			profile.showFPS = (bool)std::stoi(tokens[1]);
		}
		else if (tokens[0].compare("fps_profiling") == 0)
		{
			profile.fpsProfiling = (bool)std::stoi(tokens[1]);
		}
		else
		{
			Logger::warnln("Unknown token:", tokens[0]);
		}
	}

	file.close();
}

void Config::createDefault()
{
	Profile def;
	std::string s = "# Photon Box Configuration file\n"
					"appName = "+ def.appName +"\n"
					"width = " + std::to_string(def.width) + "\n"
					 "height = " + std::to_string(def.height) + "\n"
					 "fullscreen = " + std::to_string(def.fullscreen) + "\n"
					 "vsync = " + std::to_string(def.vsync) + "\n"
					 "supersampling = " + std::to_string(def.supersampling) + "\n"
					 "show_fps = " + std::to_string(def.showFPS) + "\n"
					 "fps_profiling = " + std::to_string(def.fpsProfiling) + "\n";

	std::ofstream file(FILE_PATH);
	file << s;
	file.close();
}
