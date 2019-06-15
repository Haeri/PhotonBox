#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ILazyLoadable;

#include <vector>

class ResourceManager
{
public:
	static unsigned int max_loadtime;
	static void lazyLoad(bool force = false);
	static bool allReady();
	static void addToInitializationList(ILazyLoadable* resource);
	static void reset();
private:
	static std::vector<ILazyLoadable*> _initializationList;
	static std::vector<ILazyLoadable*> _readyList;

	static void updateReadyList();
};

#endif // RESOURCE_MANAGER_H
