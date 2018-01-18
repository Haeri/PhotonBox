#ifndef PATH_WALKER_SCRIPT_H
#define PATH_WALKER_SCRIPT_H

#include "../Components/Behaviour.h"
#include "../Components/Transform.h"
#include "../Core/Time.h"

class PathWalkerScript : public Behaviour
{
public:
	int index = 0;
	float speed = 1;
	std::vector<Transform> path;

	void Start()
	{
		transform->setPosition(path[0].getPosition());
		transform->setRotation(path[0].getRotation());
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
			if (index + 2 >= path.size())
			{
				index = 0;
			}
			else
			{
				index += 2;
			}
		}

		transform->setPosition(Vector3f::lerp(path[index].getPosition(), path[index + 1].getPosition(), progress));
		transform->setRotation(Vector3f::lerp(path[index].getRotation(), path[index + 1].getRotation(), progress));
	}
private:
	float progress = 0;
};

#endif // PATH_WALKER_SCRIPT_H