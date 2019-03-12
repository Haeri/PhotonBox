#ifndef LOGIC_H
#define LOGIC_H

class Behaviour;

#include <vector>
#include <string>

#include "PhotonBox/core/ISystem.h"

class Logic : public ISystem
{
public:
	static void addToUpdateList(Behaviour *behaviour);
	static void removeFromList(Behaviour *behaviour);
	static unsigned long int getTickIndex();

	void start() override;
	void reset() override;
	void destroy() override;

	void tick();
	void fixedUpdate();
	void update();
	void lateUpdate();
private:
	static std::vector<Behaviour*> _behaviourList;
	static unsigned long int _tickIndex;
};

#endif // LOGIC_H
