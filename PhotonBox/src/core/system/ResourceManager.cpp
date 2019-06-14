#include "PhotonBox/core/system/ResourceManager.h"

#include <chrono>

#include "PhotonBox/core/ILazyLoadable.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

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
	
	ImGui::Begin("Assets Loader");
	for (int i = 0; i < _initializationList.size(); ++i)
	{
		ImGui::TextUnformatted((std::to_string(i+1) + " " + _initializationList[i]->getFilePath()).c_str());
	}
	ImGui::End();

	auto start = std::chrono::system_clock::now();
	while (_initializationList.size() > 0)
	{
		for (int i = static_cast<int>(_initializationList.size() - 1); i >= 0; --i)
		{
			if (_initializationList[i]->isLoaded()) {
				_initializationList[i]->initialize();

				_initializationList.erase(_initializationList.begin() + i);

				auto check = std::chrono::system_clock::now();
				if ((check - start).count() > 30000000 && !force) {
					Logger::logn("Delaying initialization to next frame\n");
					return;
				}
			}
		}
	}
}

void ResourceManager::addToInitializationList(ILazyLoadable * resource)
{
	_initializationList.push_back(resource);
}

void ResourceManager::reset()
{
	_initializationList.clear();
}
