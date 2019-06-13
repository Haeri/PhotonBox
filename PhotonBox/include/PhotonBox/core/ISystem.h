#ifndef SYSTEM_H
#define SYSTEM_H

#include "PhotonBox/core/Config.h"

class ISystem
{
public:
	virtual void init(Config::Profile profile) {}
	virtual void start() = 0;
	virtual void reset() {}
	virtual void destroy() = 0;
};

#endif // SYSTEM_H
