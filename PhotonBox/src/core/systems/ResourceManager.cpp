#include "PhotonBox/core/systems/ResourceManager.h"

#include <chrono>
#include <iostream>

#include "PhotonBox/core/ILazyLoadable.h"

std::vector<ILazyLoadable*> ResourceManager::_initializationList;

bool ResourceManager::allReady()
{
	return _initializationList.size() == 0;
}

void ResourceManager::lazyLoad(bool force)
{
	if (allReady()) { 
		return;
	}

	auto start = std::chrono::system_clock::now();
	for (int i = _initializationList.size()-1; i >= 0; --i)
	{
		if (_initializationList[i]->isLoaded()) {
			_initializationList[i]->sendToGPU();

			_initializationList.erase(_initializationList.begin() + i);

			auto check = std::chrono::system_clock::now();
			if ((check - start).count() > 30000000 && !force) {
				std::cout << "Delaying initialization to next frame\n";
				return;
			}
		}
	}
}

void ResourceManager::addToInitializationList(ILazyLoadable * resource)
{
	_initializationList.push_back(resource);
}