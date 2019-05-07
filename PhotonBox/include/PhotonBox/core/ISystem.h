#ifndef SYSTEM_H
#define SYSTEM_H

class ISystem
{
public:
	virtual void init() {}
	virtual void start() = 0;
	virtual void reset() {}
	virtual void destroy() = 0;
};

#endif // SYSTEM_H
