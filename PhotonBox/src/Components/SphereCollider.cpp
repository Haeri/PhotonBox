#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "../Core/Entity.h"
#include "../Math/Vector3f.h"
#include "Behaviour.h"
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	bool collide(Collider* other)
	{
		Vector3f pos1 = entity->getComponent<Transform>()->getPosition();
		Vector3f pos2 = other->entity->getComponent<Transform>()->getPosition();

		float minDistance = getRadius() + other->entity->getComponent<SphereCollider>()->getRadius();
		if ((pos1 - pos2).lengthSqrd() <= minDistance * minDistance)
		{
			return true;
		}

		return false;
	}

	void setRadius(float r)
	{
		_radius = r;
	}

	float getRadius()
	{
		return _radius;
	}

private:
	float _radius;

};

#endif /* defined(SPHERE_COLLIDER_H) */

