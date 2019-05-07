#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

namespace physx
{
	class PxGeometry;
	class PxSphereGeometry;
}

#include "PhotonBox/component/Collider.h"

class SphereCollider : public Collider
{
public:
	~SphereCollider();

	void init() override;
	physx::PxGeometry* getShape() override;
	void setRadius(float r);
	float getRadius() {	return _radius; }
private:
	float _radius = 1;
	physx::PxSphereGeometry* _geometry;
};

#endif // SPHERE_COLLIDER_H
