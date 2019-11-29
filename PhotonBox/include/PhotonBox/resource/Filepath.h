#ifndef FILEPATH_H
#define FILEPATH_H

#include <string>

class Filepath
{
public:
	Filepath(const std::string& filePath);
	
	std::string getAbsolutePath();
	std::string getExtension();
	std::string getName();
	std::string getPath();
private:
	std::string _absolutePath;
	std::string _path;
	std::string _name;
	std::string _extension;
};

#endif // MESH_H
