#define _USE_MATH_DEFINES
#include <math.h>

#include "../Math/Matrix4f.h"

const Matrix4f Matrix4f::IDENTITY = createIdentity();

Matrix4f Matrix4f::createRotation(float angle, Vector3f axis) {
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

Matrix4f Matrix4f::createScaling(float x, float y, float z) {
	Matrix4f ret;

	ret(0, 0) = x;	ret(1, 0) = 0;	ret(2, 0) = 0;	ret(3, 0) = 0;
	ret(0, 1) = 0;	ret(1, 1) = y;	ret(2, 1) = 0;	ret(3, 1) = 0;
	ret(0, 2) = 0;	ret(1, 2) = 0;	ret(2, 2) = z;	ret(3, 2) = 0;
	ret(0, 3) = 0;	ret(1, 3) = 0;	ret(2, 3) = 0;	ret(3, 3) = 1;

	return ret;
}

Matrix4f Matrix4f::createScaling(Vector3f vec) {
	return createScaling(vec.x(), vec.y(), vec.z());
}

Matrix4f Matrix4f::createTranslation(float x, float y, float z) {
	Matrix4f ret;
	
	ret(0, 0) = 1;	ret(1, 0) = 0;	ret(2, 0) = 0;	ret(3, 0) = x;
	ret(0, 1) = 0;	ret(1, 1) = 1;	ret(2, 1) = 0;	ret(3, 1) = y;
	ret(0, 2) = 0;	ret(1, 2) = 0;	ret(2, 2) = 1;	ret(3, 2) = z;
	ret(0, 3) = 0;	ret(1, 3) = 0;	ret(2, 3) = 0;	ret(3, 3) = 1;

	return ret;
}

Matrix4f Matrix4f::createTranslation(Vector3f vec) {
	return createTranslation(vec.x(), vec.y(), vec.z());
}

Matrix4f Matrix4f::createPerspective(float fov, float aspect, float near, float far)
{
	// General form of the Projection Matrix
	//
	// uh = Cot( fov/2 ) == 1/Tan(fov/2)
	// uw / uh = 1/aspect
	// 
	//   uw         0       0       0
	//    0        uh       0       0
	//    0         0      f/(f-n)  1
	//    0         0    -fn/(f-n)  0
	//
	// Make result to be identity first

	// check for bad parameters to avoid divide by zero:
	// if found, assert and return an identity matrix.

	float f = 1.0f / tan(fov * (M_PI / 360.0));

	Matrix4f ret;
	ret(0, 0) = f / aspect;		ret(1, 0) = 0;	ret(2, 0) = 0;									ret(3, 0) = 0;
	ret(0, 1) = 0;				ret(1, 1) = f;	ret(2, 1) = 0;									ret(3, 1) = 0;
	ret(0, 2) = 0;				ret(1, 2) = 0;	ret(2, 2) = (far + near) / (near - far);		ret(3, 2) = -1.0f;
	ret(0, 3) = 0;				ret(1, 3) = 0;	ret(2, 3) = (2.0f * far * near) / (near - far); ret(3, 3) = 0;


	/*
	float frustumDepth = far - near;
	float oneOverDepth = 1.0f / frustumDepth;
	float uh = 1.0f / tan(0.5f * fov);

	ret(0, 0) = 1.0f * uh / aspect;
	ret(1, 1) = uh;
//	ret(2, 2) = far * oneOverDepth;
//	ret(3, 2) = (-far * near) * oneOverDepth;
	ret(2, 2) = - (far + near) / (far - near);
	ret(2, 3) = -((2.0f * far * near) / (far - near));
	ret(3, 2) = - 1.0f;
	ret(3, 3) = 0;
	*/
 
	/*
	float tanHalfFOV = (float)Math.tan(fov / 2);
	float zRange = zNear - zFar;

	m[0][0] = 1.0f / (tanHalfFOV * aspectRatio);	m[0][1] = 0;					m[0][2] = 0;	m[0][3] = 0;
	m[1][0] = 0;						m[1][1] = 1.0f / tanHalfFOV;	m[1][2] = 0;	m[1][3] = 0;
	m[2][0] = 0;						m[2][1] = 0;					m[2][2] = (-zNear -zFar)/zRange;	m[2][3] = 2 * zFar * zNear / zRange;
	m[3][0] = 0;						m[3][1] = 0;					m[3][2] = 1;	m[3][3] = 0;

	*/


	return ret;
}

Matrix4f Matrix4f::lookAt(Vector3f pos, Vector3f up, Vector3f forward)
{
	Matrix4f ret;

	Vector3f zaxis = forward.normalize();
	Vector3f xaxis = (zaxis.cross(up)).normalize();
	Vector3f yaxis = xaxis.cross(zaxis);

	ret(0, 0) = xaxis.x();	ret(1, 0) = yaxis.x();	ret(2, 0) = -zaxis.x();	ret(3, 0) = 0;
	ret(0, 1) = xaxis.y();	ret(1, 1) = yaxis.y();	ret(2, 1) = -zaxis.y();	ret(3, 1) = 0;
	ret(0, 2) = xaxis.z();	ret(1, 2) = yaxis.z();	ret(2, 2) = -zaxis.z();	ret(3, 2) = 0;
	
	ret(0, 3) = -(xaxis.dot(pos));
	ret(1, 3) = -(yaxis.dot(pos));
	ret(2, 3) = (zaxis.dot(pos));
	ret(3, 3) = 1;

	return ret;
}

Matrix4f Matrix4f::createIdentity() {
	Matrix4f ret;

	ret(0,0) = 1;	ret(1, 0) = 0;	ret(2,0) = 0;	ret(3,0) = 0;
	ret(0,1) = 0;	ret(1, 1) = 1;	ret(2,1) = 0;	ret(3,1) = 0;
	ret(0,2) = 0;	ret(1, 2) = 0;	ret(2,2) = 1;	ret(3,2) = 0;
	ret(0,3) = 0;	ret(1, 3) = 0;	ret(2,3) = 0;	ret(3,3) = 1;

	return ret;
}

float Matrix4f::at(unsigned int  x, unsigned int  y)const {	
	return _matrix[x + 4 * y];
}

void Matrix4f::set(unsigned int x, unsigned int  y, float val) {
	_matrix[x + 4 * y] = val;
}

Matrix4f Matrix4f::transpose() {
	Matrix4f temp;

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			temp(j, i) = this->at(i, j);
		}
	}
	for (size_t i = 0; i < 16; ++i) {
		this->_matrix[i] = temp._matrix[i];
	}
	return *this;
}
