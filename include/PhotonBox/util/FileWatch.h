#ifndef FILE_WATCH_H
#define FILE_WATCH_H

class Shader;
class LazyLoadable;

#include <iostream>
#include <map>
#include <string>
#include <thread>

#include <sys/types.h>
#include <sys/stat.h>

class FileWatch
{
public:
	struct ResourceFile
	{
		int64_t stamp;
		LazyLoadable* resource;
	};

	static void addToWatchList(std::string filePath, Shader* shader);
	static void addToWatchList(std::string filePath, LazyLoadable* resource);
	void checkValidity();
	void reset();
	void destroy();
private:
	static bool _loading;
	static std::thread _thread;
	static std::map<std::string, ResourceFile> _watchList;

	void asyncCheck();
};

#endif // FILE_WATCH_H
