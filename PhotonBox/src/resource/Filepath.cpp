#include "PhotonBox/resource/Filepath.h"

#include <algorithm>
#include <string>

Filepath::Filepath(const std::string& filePath)
{
	_absolutePath = filePath;

	std::replace(_absolutePath.begin(), _absolutePath.end(), '\\', '/');
	
	std::size_t lastDot = _absolutePath.find_last_of(".");
	std::size_t lastDash = _absolutePath.find_last_of("/");
	std::size_t lastDash1 = lastDash + 1;

	if (lastDot == std::string::npos || lastDot < lastDash) {
		// No extension

		_extension = "";
		_name = _absolutePath.substr(lastDash1, _absolutePath.length() - lastDash1);
		_path = _absolutePath.substr(0, lastDash1);
	}
	else 
	{
		_extension = _absolutePath.substr(lastDot);
		_name = _absolutePath.substr(lastDash1, lastDot - lastDash1);
		_path = _absolutePath.substr(0, lastDash1);
	}

	
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
