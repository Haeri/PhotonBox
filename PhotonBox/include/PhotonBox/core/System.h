#ifndef SYSTEM_H
#define SYSTEM_H

class System
{
public:
	virtual void start() = 0;
	virtual void update() {}
	virtual void destroy() = 0;
};

#endif // SYSTEM_H
