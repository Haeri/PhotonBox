#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"
#include "Behaviour.h"

class SphereCollider : public Collider {
public:
	float radius;
    
	bool collide(Collider* other) override {
        vmml::Vector3f pos1 = gameObject->getComponent<Transform>()->position;
        vmml::Vector3f pos2 = other->gameObject->getComponent<Transform>()->position;

		float minDistance = radius + other->gameObject->getComponent<SphereCollider>()->radius;
        if((pos1 - pos2).squared_length() <= minDistance * minDistance){
            std::cout << gameObject->name << " collidet with " << other->gameObject->name << std::endl;
            return true;
        }
        return false;
    }
};

#endif /* defined(SPHERE_COLLIDER_H) */
