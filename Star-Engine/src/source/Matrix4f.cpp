#include "../header/Matrix4f.h"

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

Matrix4f Matrix4f::createIdentity() {
	Matrix4f ret;

	ret(0,0) = 1;	ret(1, 0) = 0;	ret(2,0) = 0;	ret(3,0) = 0;
	ret(0,1) = 0;	ret(1, 1) = 1;	ret(2,1) = 0;	ret(3,1) = 0;
	ret(0,2) = 0;	ret(1, 2) = 0;	ret(2,2) = 1;	ret(3,2) = 0;
	ret(0,3) = 0;	ret(1, 3) = 0;	ret(2,3) = 0;	ret(3,3) = 1;

	return ret;
}

const float Matrix4f::at(unsigned int  x, unsigned int  y) const {
	float ret = (x, y);
	return ret;
}

void Matrix4f::set(unsigned int x, unsigned int  y, float val) {
	(x, y) = val;
}