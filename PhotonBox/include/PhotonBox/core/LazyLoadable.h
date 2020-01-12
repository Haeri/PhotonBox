#ifndef LAZY_LOADABLE_H
#define LAZY_LOADABLE_H

#include <string>

#include "PhotonBox/resource/Filepath.h"

class LazyLoadable
{
public:
	enum State 
	{
		UNSET,
		LOADING,
		LOADED,
		FAILED,
		INITIALIZED
	};

	virtual ~LazyLoadable() {}

	void loadAsync();
	void initialize();
	void setRogue();
	void forceLoad();

	State getState();
	bool isLoaded();
	bool isInitialized();

	virtual std::string getFilePath();
protected:
	Filepath _filePath;

	virtual bool loadFromFile() = 0;
	virtual void submitBuffer() = 0;
	virtual void blankInitialize() {}
private:
	State _state = State::UNSET;
	bool _rogue = false;

	void load();
};

#endif // LAZY_LOADABLE_H
