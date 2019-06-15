#include "PhotonBox/core/system/ResourceManager.h"

#include <chrono>
#include <algorithm>

#include "PhotonBox/core/ILazyLoadable.h"
#include "PhotonBox/core/system/DebugGUI.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

std::vector<ILazyLoadable*> ResourceManager::_initializationList;
std::vector<ILazyLoadable*> ResourceManager::_readyList;
unsigned int ResourceManager::max_loadtime = 300000;

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
		Logger::logn("Loop", Logger::WARN);
		updateReadyList();

		for (int i = static_cast<int>(_readyList.size() - 1); i >= 0; --i)
		{
			Logger::logn(_readyList[i]->getFilePath(), Logger::CONFIRM);
			
			_readyList[i]->initialize();
			_initializationList.erase(std::remove(_initializationList.begin(), _initializationList.end(), _readyList[i]), _initializationList.end());
			_readyList.erase(_readyList.begin() + i);
				

			// Escape if time is up
			auto check = std::chrono::system_clock::now();
			if ((check - start).count() > max_loadtime && !force) {
				Logger::logn("Delaying initialization: " + std::to_string((check - start).count()), Logger::WARN);
				return;
			}
		}

		if (!force) return;
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

void ResourceManager::updateReadyList()
{
	for (int i = 0; i < _initializationList.size(); ++i)
	{
		if (_initializationList[i]->isLoaded() &&
			std::find(_readyList.begin(), _readyList.end(), _initializationList[i]) == _readyList.end())
		{
			_readyList.push_back(_initializationList[i]);
		}
	}
}

