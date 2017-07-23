#ifndef LOGIC_H
#define LOGIC_H

class Behaviour;
#include <vector>

class Logic {
public:
	void start();
	void update();
	void fixedUpdate();
	void lateUpdate();
	void destroy();
	void addToUpdateList(Behaviour *behaviour);
	void removeFromList(Behaviour *behaviour);
	void printList();
	std::string getList();
private:
	std::vector<Behaviour*> behaviourList;
};

#endif /* defined(LOGIC_H) */