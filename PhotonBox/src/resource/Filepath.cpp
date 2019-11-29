#include <algorithm>

#include "PhotonBox/resource/Filepath.h"

Filepath::Filepath(const std::string& filePath)
{
	_absolutePath = filePath;

	std::replace(_absolutePath.begin(), _absolutePath.end(), '\\', '/');

	std::size_t lastDot = _absolutePath.find_last_of(".");
	_extension = _absolutePath.substr(lastDot);

	std::size_t lastDash = _absolutePath.find_last_of("/");
	_name = _absolutePath.substr(lastDash, lastDot);

	_path = _absolutePath.substr(0, lastDash + 1);
}

std::string Filepath::getAbsolutePath()
{
	return _absolutePath;
}

std::string Filepath::getExtension()
{

	return _extension;
}

std::string Filepath::getName()
{
	return _name;
}

std::string Filepath::getPath()
{
	return _path;
}
