#ifndef LOGIC_H
#define LOGIC_H

class Behaviour;
#include <vector>

class Logic {
public:
	static void addToUpdateList(Behaviour *behaviour);
	static void removeFromList(Behaviour *behaviour);
	static void printList();
	static std::string getList();

	void start();
	void update();
	void fixedUpdate();
	void lateUpdate();
	void destroy();
private:
	static std::vector<Behaviour*> _behaviourList;
};

#endif // LOGIC_H