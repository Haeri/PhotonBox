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
	struct IndexedLazy
	{
		int index;
		ILazyLoadable* resource;
	};
	static std::vector<ILazyLoadable*> _initializationList;
};

#endif // RESOURCE_MANAGER_H
