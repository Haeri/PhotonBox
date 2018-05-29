#ifndef COLLIDER_H
#define COLLIDER_H

class PxGeometry;
#include "../Core/Component.h"

class Collider : public Component
{
public:
	Collider();
	void destroy();

	virtual PxGeometry* getShape() = 0;
};

#endif /* defined(COLLIDER_H) */
