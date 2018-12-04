#ifndef PATH_WALKER_SCRIPT_CPP
#define PATH_WALKER_SCRIPT_CPP

#include <components/Behaviour.h>
#include <components/Transform.h>
#include <core/Time.h>

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class PathWalkerScript : public Behaviour
{
public:
	int index = 0;
	float speed = 1;
	//std::vector<Transform> path;
	std::vector<Vector3f> positions;
	std::vector<Quaternion> rotations;

	void Start()
	{
		transform->setPosition(positions[0]);
		transform->setRotation(rotations[0]);
	}

	void Update()
	{

		if (progress < 1)
		{
			progress += Time::deltaTime * speed;
		}
		else
		{
			progress = 0;
			if (index + 2 >= positions.size())
			{
				index = 0;
			}
			else
			{
				index += 2;
			}
		}

		transform->setPosition(Vector3f::lerp(positions[index], positions[index + 1], progress));
		transform->setRotation(Quaternion::lerp(rotations[index], rotations[index + 1], progress));
	}
private:
	float progress = 0;
};

#endif // PATH_WALKER_SCRIPT_CPP