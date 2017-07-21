#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

class Collider : public Component {
public:
	Collider();
	void destroy();

	virtual bool collide(Collider* other) =0;
};

#endif /* defined(COLLIDER_H) */