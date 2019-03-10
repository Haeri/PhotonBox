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
	static void printList();
	static std::string getList();

	void start() override;
	void destroy() override;

	void fixedUpdate();
	void update();
	void lateUpdate();
private:
	static std::vector<Behaviour*> _behaviourList;
};

#endif // LOGIC_H
