#ifndef LOGIC_H
#define LOGIC_H

class Behaviour;
#include <vector>

class Logic {
public:
	static void start();
	static void update();
	static void fixedUpdate();
	static void lateUpdate();
	static void destroy();
	static void addToUpdateList(Behaviour *behaviour);
	static void removeFromList(Behaviour *behaviour);
	static void printList();
	static std::string getList();
private:
	static std::vector<Behaviour*> behaviourList;
};

#endif /* defined(LOGIC_H) */