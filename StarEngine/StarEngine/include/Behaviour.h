#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Gameobject.h"
#include "Component.h"
#include "Logic.h"

class Behaviour: public Component {
public:
	Behaviour();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void OnDestroy();

	void destroy();
private:
	
};

#endif /* defined(BEHAVIOUR_H) */