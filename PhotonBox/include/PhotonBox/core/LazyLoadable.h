#ifndef LAZY_LOADABLE_H
#define LAZY_LOADABLE_H

#include <string>

#include "PhotonBox/resource/Filepath.h"

class LazyLoadable
{
public:
	virtual ~LazyLoadable() {}

	void loadAsync();
	void initialize();
	void setRogue();
	void forceLoad();

	bool isLoaded();
	bool isInitialized();

	virtual std::string getFilePath();
protected:
	bool _isLoaded = false;
	bool _isInitialized = false;
	Filepath _filePath;

	virtual void loadFromFile() = 0;
	virtual void submitBuffer() = 0;
	virtual void blankInitialize() {}
private:
	bool _rogue = false;

	void load();
};

#endif // LAZY_LOADABLE_H
