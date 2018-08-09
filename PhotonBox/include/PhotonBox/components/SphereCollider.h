#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "PhotonBox/components/Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider();

	physx::PxGeometry* getShape() override;
	void setRadius(float r);
	float getRadius() {	return _radius; }
private:
	float _radius;
	physx::PxSphereGeometry* _geometry;
};

#endif // SPHERE_COLLIDER_H
