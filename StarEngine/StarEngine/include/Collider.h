#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

class Collider : public Component {
public:
	Collider();
	virtual bool collide(Collider* other) =0;
	void destroy();
};

#endif /* defined(COLLIDER_H) */