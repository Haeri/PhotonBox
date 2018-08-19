#include "PhotonBox/math/Vector2f.h"

const Vector2f Vector2f::ZERO = Vector2f(0, 0);
const Vector2f Vector2f::ONE = Vector2f(1, 1);
const Vector2f Vector2f::UNIT_X = Vector2f(1, 0);
const Vector2f Vector2f::UNIT_Y = Vector2f(0, 1);
const Vector2f Vector2f::UP = Vector2f(0, 1);
const Vector2f Vector2f::DOWN = Vector2f(0, -1);
const Vector2f Vector2f::LEFT = Vector2f(-1, 0);
const Vector2f Vector2f::RIGHT = Vector2f(1, 0);

Vector2f Vector2f::lerp(Vector2f start, Vector2f target, float percent)
{
	return Vector2f(
		start.x() + percent * (target.x() - start.x()),
		start.y() + percent * (target.y() - start.y())
	);
}
