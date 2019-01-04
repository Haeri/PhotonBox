#ifndef I_LAZY_LOADABLE_H
#define I_LAZY_LOADABLE_H

class ILazyLoadable
{
public:
	virtual ~ILazyLoadable() {}

	void loadAsync();

	virtual void sendToGPU() = 0;

	bool isLoaded() { return _isLoaded; }
	bool isInitialized() { return _isInitialized; }
protected:
	bool _isLoaded = false;
	bool _isInitialized = false;
	

	virtual void blankInitialize() {}
	virtual void loadFromFile() = 0;
};

#endif // I_LAZY_LOADABLE_H
