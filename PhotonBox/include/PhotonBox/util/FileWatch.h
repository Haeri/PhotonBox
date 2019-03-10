#ifndef FILE_WATCH_H
#define FILE_WATCH_H

class Shader;
class ILazyLoadable;

#include <iostream>
#include <map>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
//#define stat _stat


class FileWatch
{
public:
	struct ResourceFile
	{
		int64_t stamp;
		ILazyLoadable* resource;
	};

	static void addToWatchList(std::string filePath, Shader* shader);
	static void addToWatchList(std::string filePath, ILazyLoadable* resource);
	void checkValidity();
private:
	static bool _loading;
	void asyncCheck();
	static std::map<std::string, ResourceFile> _watchList;
};

#endif // FILE_WATCH_H
