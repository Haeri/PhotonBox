#ifndef VERTEX_H
#define VERTEX_H

#include "PhotonBox/math/Vector2f.h"
#include "PhotonBox/math/Vector3f.h"

class Vertex
{
public:
	enum AttibLocation
	{
		POSITION,
		NORMAL,
		COLOR,
		TEXTURECOORD,
		TANGENT,
	};

	Vector3f position;
	Vector3f normal;
	Vector3f color;
	Vector2f uv;
	Vector3f tangent;

	Vertex() {}
	Vertex(Vector3f pos, Vector3f normal, Vector3f col, Vector2f uv) :
		position(pos),
		normal(normal),
		color(col),
		uv(uv)
	{
	}
	Vertex(Vector3f pos, Vector3f normal, Vector3f col, Vector2f uv, Vector3f tangent) :
		position(pos),
		normal(normal),
		color(col),
		uv(uv),
		tangent(tangent)
	{
	}
};

#endif /* defined(VERTEX_H) */
