#include "PhotonBox/components/Rigidbody.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void Rigidbody::setBody(PxRigidBody * rigidBody)
{
	_rigidBody = rigidBody;
}

void Rigidbody::setDensity(float mass)
{
	_mass = mass;
	_rigidBody->setMass(mass);
}

void Rigidbody::addForce(Vector3f direction)
{
	if(_rigidBody != nullptr)
		_rigidBody->addForce(PxVec3(direction.getX(), direction.getY(), direction.getZ()));
}
