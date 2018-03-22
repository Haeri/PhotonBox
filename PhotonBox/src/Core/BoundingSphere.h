#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "../Math/Vector3f.h"

class BoundingSphere
{
public:
	BoundingSphere() {}
	BoundingSphere(Vector3f center, float radius): _center(center), _radius(radius) {}
	Vector3f getCenter() { return _center; };
	float getRadius() { return _radius; }
private:
	Vector3f _center;
	float _radius;
};

#endif // BOUNDING_SPHERE_H
