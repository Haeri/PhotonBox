#define _USE_MATH_DEFINES
#include <math.h>

#include "Matrix4f.h"

const Matrix4f Matrix4f::IDENTITY = createIdentity();

Matrix4f Matrix4f::createRotation(float angle, Vector3f axis)
{
	Matrix4f ret;

	const float sine = std::sin(angle);
	const float cosine = std::cos(angle);

	const float zero = 0.0;
	const float one = 1.0;
	const float two = 2.0;

	ret._matrix[0] = cosine + (one - cosine) * std::pow(axis[0], two);
	ret._matrix[1] = (one - cosine) * axis[0] * axis[1] + sine * axis[2];
	ret._matrix[2] = (one - cosine) * axis[0] * axis[2] - sine * axis[1];
	ret._matrix[3] = zero;

	ret._matrix[4] = (one - cosine) * axis[0] * axis[1] - sine * axis[2];
	ret._matrix[5] = cosine + (one - cosine) * std::pow(axis[1], two);
	ret._matrix[6] = (one - cosine) * axis[1] * axis[2] + sine * axis[0];
	ret._matrix[7] = zero;

	ret._matrix[8] = (one - cosine) * axis[0] * axis[2] + sine * axis[1];
	ret._matrix[9] = (one - cosine) * axis[1] * axis[2] - sine * axis[0];
	ret._matrix[10] = cosine + (one - cosine) * std::pow(axis[2], two);
	ret._matrix[11] = zero;

	ret._matrix[12] = zero;
	ret._matrix[13] = zero;
	ret._matrix[14] = zero;
	ret._matrix[15] = one;

	return ret;
}

Matrix4f Matrix4f::createScaling(float x, float y, float z)
{
	Matrix4f ret;
	ret._matrix[0] = x;		ret._matrix[4] = 0;		ret._matrix[8] = 0;		ret._matrix[12] = 0;
	ret._matrix[1] = 0;		ret._matrix[5] = y;		ret._matrix[9] = 0;		ret._matrix[13] = 0;
	ret._matrix[2] = 0;		ret._matrix[6] = 0;		ret._matrix[10] = z;	ret._matrix[14] = 0;
	ret._matrix[3] = 0;		ret._matrix[7] = 0;		ret._matrix[11] = 0;	ret._matrix[15] = 1;
	return ret;
}

Matrix4f Matrix4f::createScaling(Vector3f vec)
{
	return createScaling(vec.x(), vec.y(), vec.z());
}

Matrix4f Matrix4f::createTranslation(float x, float y, float z)
{
	Matrix4f ret;
	ret._matrix[0] = 1;		ret._matrix[4] = 0;		ret._matrix[8] = 0;		ret._matrix[12] = x;
	ret._matrix[1] = 0;		ret._matrix[5] = 1;		ret._matrix[9] = 0;		ret._matrix[13] = y;
	ret._matrix[2] = 0;		ret._matrix[6] = 0;		ret._matrix[10] = 1;	ret._matrix[14] = z;
	ret._matrix[3] = 0;		ret._matrix[7] = 0;		ret._matrix[11] = 0;	ret._matrix[15] = 1;
	return ret;
}

Matrix4f Matrix4f::createTranslation(Vector3f vec)
{
	return createTranslation(vec.x(), vec.y(), vec.z());
}

Matrix4f Matrix4f::createPerspective(float fov, float aspect, float near, float far)
{
	float tanHalfFovy = tan((fov / 360 * 2 * M_PI) / 2);

	Matrix4f ret;
	ret._matrix[0] = 1.0f / (aspect * tanHalfFovy);	ret._matrix[4] = 0;							ret._matrix[8] = 0;								ret._matrix[12] = 0;
	ret._matrix[1] = 0;								ret._matrix[5] = 1.0f / (tanHalfFovy);		ret._matrix[9] = 0;								ret._matrix[13] = 0;
	ret._matrix[2] = 0;								ret._matrix[6] = 0;							ret._matrix[10] = (far + near) / (near - far);	ret._matrix[14] = (2.0f * far * near) / (near - far);
	ret._matrix[3] = 0;								ret._matrix[7] = 0;							ret._matrix[11] = -1;							ret._matrix[15] = 0;
	return ret;
}

Matrix4f Matrix4f::createOrthographic(float left, float right, float bottom, float top)
{
	Matrix4f ret;
	ret._matrix[0] = 2.0f / (right - left);	ret._matrix[4] = 0;							ret._matrix[8] = 0;		ret._matrix[12] = -(right + left) / (right - left);
	ret._matrix[1] = 0;						ret._matrix[5] = 2.0f / (top - bottom);		ret._matrix[9] = 0;		ret._matrix[13] = -(top + bottom) / (top - bottom);
	ret._matrix[2] = 0;						ret._matrix[6] = 0;							ret._matrix[10] = 1; 	ret._matrix[14] = 0;
	ret._matrix[3] = 0;						ret._matrix[7] = 0;							ret._matrix[11] = 0;	ret._matrix[15] = 1;
	return ret;
}

Matrix4f Matrix4f::createOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	Matrix4f ret;
	ret._matrix[0] = 2.0f / (right - left);	ret._matrix[4] = 0;							ret._matrix[8] = 0;							ret._matrix[12] = -(right + left) / (right - left);
	ret._matrix[1] = 0;						ret._matrix[5] = 2.0f / (top - bottom);		ret._matrix[9] = 0;							ret._matrix[13] = -(top + bottom) / (top - bottom);
	ret._matrix[2] = 0;						ret._matrix[6] = 0;							ret._matrix[10] = -2.0f / (far - near);		ret._matrix[14] = -(far + near) / (far - near);
	ret._matrix[3] = 0;						ret._matrix[7] = 0;							ret._matrix[11] = 0;						ret._matrix[15] = 1;
	return ret;
}

Matrix4f Matrix4f::lookAt(Vector3f pos, Vector3f up, Vector3f forward)
{

	Vector3f zaxis = forward.normalize();
	Vector3f xaxis = (zaxis.cross(up)).normalize();
	Vector3f yaxis = xaxis.cross(zaxis);

	Matrix4f ret;
	ret._matrix[0] = xaxis.x();		ret._matrix[4] = xaxis.y();		ret._matrix[8] = xaxis.z();		ret._matrix[12] = -(xaxis.dot(pos));
	ret._matrix[1] = yaxis.x();		ret._matrix[5] = yaxis.y();		ret._matrix[9] = yaxis.z();		ret._matrix[13] = -(yaxis.dot(pos));
	ret._matrix[2] = -zaxis.x();	ret._matrix[6] = -zaxis.y();	ret._matrix[10] = -zaxis.z();	ret._matrix[14] = (zaxis.dot(pos));
	ret._matrix[3] = 0;				ret._matrix[7] = 0;				ret._matrix[11] = 0;			ret._matrix[15] = 1;
	return ret;
}

Matrix4f Matrix4f::createIdentity()
{
	Matrix4f ret;
	ret._matrix[0] = 1;		ret._matrix[4] = 0;		ret._matrix[8] = 0;		ret._matrix[12] = 0;
	ret._matrix[1] = 0;		ret._matrix[5] = 1;		ret._matrix[9] = 0;		ret._matrix[13] = 0;
	ret._matrix[2] = 0;		ret._matrix[6] = 0;		ret._matrix[10] = 1;	ret._matrix[14] = 0;
	ret._matrix[3] = 0;		ret._matrix[7] = 0;		ret._matrix[11] = 0;	ret._matrix[15] = 1;
	return ret;
}

float Matrix4f::at(unsigned int  x, unsigned int  y)const
{
	return _matrix[y + 4 * x];
}

void Matrix4f::set(unsigned int x, unsigned int  y, float val)
{
	_matrix[y + 4 * x] = val;
}

Matrix4f Matrix4f::transpose()
{
	Matrix4f temp;

	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			temp(j, i) = this->at(i, j);
		}
	}
	for (size_t i = 0; i < 16; ++i)
	{
		this->_matrix[i] = temp._matrix[i];
	}
	return *this;
}

Matrix4f Matrix4f::inverse()
{
	Matrix4f ret, temp;

	float det;

	temp._matrix[0] = _matrix[5] * _matrix[10] * _matrix[15] -
		_matrix[5] * _matrix[11] * _matrix[14] -
		_matrix[9] * _matrix[6] * _matrix[15] +
		_matrix[9] * _matrix[7] * _matrix[14] +
		_matrix[13] * _matrix[6] * _matrix[11] -
		_matrix[13] * _matrix[7] * _matrix[10];

	temp._matrix[4] = -_matrix[4] * _matrix[10] * _matrix[15] +
		_matrix[4] * _matrix[11] * _matrix[14] +
		_matrix[8] * _matrix[6] * _matrix[15] -
		_matrix[8] * _matrix[7] * _matrix[14] -
		_matrix[12] * _matrix[6] * _matrix[11] +
		_matrix[12] * _matrix[7] * _matrix[10];

	temp._matrix[8] = _matrix[4] * _matrix[9] * _matrix[15] -
		_matrix[4] * _matrix[11] * _matrix[13] -
		_matrix[8] * _matrix[5] * _matrix[15] +
		_matrix[8] * _matrix[7] * _matrix[13] +
		_matrix[12] * _matrix[5] * _matrix[11] -
		_matrix[12] * _matrix[7] * _matrix[9];

	temp._matrix[12] = -_matrix[4] * _matrix[9] * _matrix[14] +
		_matrix[4] * _matrix[10] * _matrix[13] +
		_matrix[8] * _matrix[5] * _matrix[14] -
		_matrix[8] * _matrix[6] * _matrix[13] -
		_matrix[12] * _matrix[5] * _matrix[10] +
		_matrix[12] * _matrix[6] * _matrix[9];

	temp._matrix[1] = -_matrix[1] * _matrix[10] * _matrix[15] +
		_matrix[1] * _matrix[11] * _matrix[14] +
		_matrix[9] * _matrix[2] * _matrix[15] -
		_matrix[9] * _matrix[3] * _matrix[14] -
		_matrix[13] * _matrix[2] * _matrix[11] +
		_matrix[13] * _matrix[3] * _matrix[10];

	temp._matrix[5] = _matrix[0] * _matrix[10] * _matrix[15] -
		_matrix[0] * _matrix[11] * _matrix[14] -
		_matrix[8] * _matrix[2] * _matrix[15] +
		_matrix[8] * _matrix[3] * _matrix[14] +
		_matrix[12] * _matrix[2] * _matrix[11] -
		_matrix[12] * _matrix[3] * _matrix[10];

	temp._matrix[9] = -_matrix[0] * _matrix[9] * _matrix[15] +
		_matrix[0] * _matrix[11] * _matrix[13] +
		_matrix[8] * _matrix[1] * _matrix[15] -
		_matrix[8] * _matrix[3] * _matrix[13] -
		_matrix[12] * _matrix[1] * _matrix[11] +
		_matrix[12] * _matrix[3] * _matrix[9];

	temp._matrix[13] = _matrix[0] * _matrix[9] * _matrix[14] -
		_matrix[0] * _matrix[10] * _matrix[13] -
		_matrix[8] * _matrix[1] * _matrix[14] +
		_matrix[8] * _matrix[2] * _matrix[13] +
		_matrix[12] * _matrix[1] * _matrix[10] -
		_matrix[12] * _matrix[2] * _matrix[9];

	temp._matrix[2] = _matrix[1] * _matrix[6] * _matrix[15] -
		_matrix[1] * _matrix[7] * _matrix[14] -
		_matrix[5] * _matrix[2] * _matrix[15] +
		_matrix[5] * _matrix[3] * _matrix[14] +
		_matrix[13] * _matrix[2] * _matrix[7] -
		_matrix[13] * _matrix[3] * _matrix[6];

	temp._matrix[6] = -_matrix[0] * _matrix[6] * _matrix[15] +
		_matrix[0] * _matrix[7] * _matrix[14] +
		_matrix[4] * _matrix[2] * _matrix[15] -
		_matrix[4] * _matrix[3] * _matrix[14] -
		_matrix[12] * _matrix[2] * _matrix[7] +
		_matrix[12] * _matrix[3] * _matrix[6];

	temp._matrix[10] = _matrix[0] * _matrix[5] * _matrix[15] -
		_matrix[0] * _matrix[7] * _matrix[13] -
		_matrix[4] * _matrix[1] * _matrix[15] +
		_matrix[4] * _matrix[3] * _matrix[13] +
		_matrix[12] * _matrix[1] * _matrix[7] -
		_matrix[12] * _matrix[3] * _matrix[5];

	temp._matrix[14] = -_matrix[0] * _matrix[5] * _matrix[14] +
		_matrix[0] * _matrix[6] * _matrix[13] +
		_matrix[4] * _matrix[1] * _matrix[14] -
		_matrix[4] * _matrix[2] * _matrix[13] -
		_matrix[12] * _matrix[1] * _matrix[6] +
		_matrix[12] * _matrix[2] * _matrix[5];

	temp._matrix[3] = -_matrix[1] * _matrix[6] * _matrix[11] +
		_matrix[1] * _matrix[7] * _matrix[10] +
		_matrix[5] * _matrix[2] * _matrix[11] -
		_matrix[5] * _matrix[3] * _matrix[10] -
		_matrix[9] * _matrix[2] * _matrix[7] +
		_matrix[9] * _matrix[3] * _matrix[6];

	temp._matrix[7] = _matrix[0] * _matrix[6] * _matrix[11] -
		_matrix[0] * _matrix[7] * _matrix[10] -
		_matrix[4] * _matrix[2] * _matrix[11] +
		_matrix[4] * _matrix[3] * _matrix[10] +
		_matrix[8] * _matrix[2] * _matrix[7] -
		_matrix[8] * _matrix[3] * _matrix[6];

	temp._matrix[11] = -_matrix[0] * _matrix[5] * _matrix[11] +
		_matrix[0] * _matrix[7] * _matrix[9] +
		_matrix[4] * _matrix[1] * _matrix[11] -
		_matrix[4] * _matrix[3] * _matrix[9] -
		_matrix[8] * _matrix[1] * _matrix[7] +
		_matrix[8] * _matrix[3] * _matrix[5];

	temp._matrix[15] = _matrix[0] * _matrix[5] * _matrix[10] -
		_matrix[0] * _matrix[6] * _matrix[9] -
		_matrix[4] * _matrix[1] * _matrix[10] +
		_matrix[4] * _matrix[2] * _matrix[9] +
		_matrix[8] * _matrix[1] * _matrix[6] -
		_matrix[8] * _matrix[2] * _matrix[5];

	det = _matrix[0] * temp._matrix[0] + _matrix[1] * temp._matrix[4] + _matrix[2] * temp._matrix[8] + _matrix[3] * temp._matrix[12];

	if (det == 0)
		return Matrix4f::IDENTITY;

	det = 1.0 / det;

	for (size_t i = 0; i < 16; i++)
		ret._matrix[i] = temp._matrix[i] * det;

	return ret;
}
