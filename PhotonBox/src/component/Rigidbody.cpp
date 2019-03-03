#include "PhotonBox/component/Rigidbody.h"

#include "PhotonBox/core/system/Physics.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

Rigidbody::Rigidbody(){
	Physics::registerObject(this);
}

Rigidbody::~Rigidbody()
{
	Physics::removePhysicsObject(this);
}

void Rigidbody::setBody(physx::PxRigidBody * rigidBody)
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
