#include "PhotonBox/resource/Filepath.h"

#include <algorithm>
#include <string>

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Filepath::Filepath(const std::string& filePath)
{
	_filePath = filePath;

	std::replace(_filePath.begin(), _filePath.end(), '\\', '/');
	
	std::size_t lastDot = _filePath.find_last_of(".");
	std::size_t lastDash = _filePath.find_last_of("/");
	std::size_t lastDash1 = lastDash + 1;

	if (lastDot == std::string::npos || lastDot < lastDash) {
		// No extension

		_extension = "";
		_name = _filePath.substr(lastDash1, _filePath.length() - lastDash1);
		_path = _filePath.substr(0, lastDash1);
	}
	else 
	{
		_extension = _filePath.substr(lastDot);
		_name = _filePath.substr(lastDash1, lastDot - lastDash1);
		_path = _filePath.substr(0, lastDash1);
	}

	
}

std::string Filepath::getFilePath()
{
	return _filePath;
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
