#ifndef I_LAZY_LOADABLE_H
#define I_LAZY_LOADABLE_H

class ILazyLoadable
{
public:
	virtual ~ILazyLoadable() {}

	void loadAsync();


	bool isLoaded() { return _isLoaded; }
	bool isInitialized() { return _isInitialized; }
	void forceLoad();
	void setRogue();
	void initialize();
protected:
	bool _isLoaded = false;
	bool _isInitialized = false;

	virtual void submitBuffer() = 0;
	virtual void blankInitialize() {}
	virtual void loadFromFile() = 0;
private:
	bool _rougue = false;

	void load();
};

#endif // I_LAZY_LOADABLE_H
