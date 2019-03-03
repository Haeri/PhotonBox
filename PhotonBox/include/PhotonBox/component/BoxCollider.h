#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "PhotonBox/component/Collider.h"
#include "PhotonBox/math/Vector3f.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();
	~BoxCollider();
	
	physx::PxGeometry* getShape() override;
	Vector3f getHalfExtents() { return _extents; }
	void setHalfExtents(Vector3f extents);
private:
	Vector3f _extents;
	physx::PxBoxGeometry* _geometry;
};

#endif // BOX_COLLIDER_H
