#include "PhotonBox/util/FileWatch.h"

#include "PhotonBox/resources/Shader.h"

std::map<std::string, FileWatch::ShaderFile> FileWatch::_watchList;

void FileWatch::addToWatchList(std::string filePath, Shader* shader)
{
	struct stat result;
	std::string s = filePath + ".fs";
	if (stat(s.c_str(), &result) == 0)
	{
		ShaderFile sf = {
			result.st_mtime,
			shader,
		};
		_watchList[s] = sf;
	}
		
	s = filePath + ".vs";
	if (stat(s.c_str(), &result) == 0)
	{
		ShaderFile sf = {
			result.st_mtime,
			shader,
		};
		_watchList[s] = sf;
	}
}

void FileWatch::checkValidity()
{
	for (std::map<std::string, ShaderFile>::iterator it = _watchList.begin(); it != _watchList.end(); ++it)
	{
		struct stat result;
		if (stat(it->first.c_str(), &result) == 0 && it->second.stamp != result.st_mtime)
		{
			std::cout << "Updating " << it->first << std::endl;
			it->second.stamp = result.st_mtime;
			it->second.shader->reload();
		}
	}
}
