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
	static void addToUpdateList(Behaviour *behaviour);
	static void printList();
	static std::string getList();
private:
	static std::vector<Behaviour*> behaviourList;
};
#endif /* defined(LOGIC_H) */