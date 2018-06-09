#include "PhotonBox/core/Util.h"
#include "PhotonBox/resources/Config.h"

#include <algorithm>
#include <fstream>
#include <vector>
#include <iostream>

Config::Profile Config::profile;

void Config::readConfig()
{
	std::ifstream file(FILE_PATH);
	std::string line;
	std::vector<std::string> tokens;

	if (file.fail())
	{
		std::cerr << FILE_PATH << " could not be found." << std::endl;
		createDefault();

		return;
	}

	while (std::getline(file, line))
	{
		tokens.clear();
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		Util::split(line, "=", tokens);

		if (tokens[0].at(0) == '#')
		{
			continue;
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
			profile.supersampling = (bool)std::stoi(tokens[1]);
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
			std::cout << "Unknown token: " << tokens[0] << std::endl;
		}
	}

	file.close();
}

void Config::createDefault()
{
	std::string def = "#Photon Box Configuration file\n"
					 "width = 1280\n"
					 "height = 720\n"
					 "fullscreen = 0\n"
					 "vsync = 1\n"
					 "supersampling = 1\n"
					 "show_fps = 1\n"
					 "fps_profiling = 0\n";

	std::ofstream file(FILE_PATH);
	file << def;
	file.close();

	readConfig();
}
