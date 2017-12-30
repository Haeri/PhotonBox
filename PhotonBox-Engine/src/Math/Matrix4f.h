#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Vector3f.h"
#include "Vector4f.h"

class Matrix4f {
public:
	const static Matrix4f IDENTITY;

	static Matrix4f createRotation(float angle, Vector3f axis);
	static Matrix4f createScaling(float x, float y, float z);
	static Matrix4f createScaling(Vector3f vec);
	static Matrix4f createTranslation(float x, float y, float z);
	static Matrix4f createTranslation(Vector3f vec);
	static Matrix4f createPerspective(float fov, float aspectRatio, float near, float far);
	static Matrix4f createOrthographic(float left, float right, float bottom, float top);
	static Matrix4f createOrthographic(float left, float right, float bottom, float top, float near, float far);
	static Matrix4f lookAt(Vector3f pos, Vector3f up, Vector3f forward);
	
	float at(unsigned int  x, unsigned int  y) const;
	void set(unsigned int x, unsigned int  y, float val);
	Matrix4f transpose();
	Matrix4f inverse();

	inline Matrix4f operator* (const Matrix4f& other) const {
		Matrix4f ret;

		for (unsigned int i = 0; i < 4; i++){
			for (unsigned int j = 0; j < 4; j++){
				ret._matrix[j * 4 + i] =	_matrix[0 * 4 + i] * other._matrix[j * 4 + 0] +
											_matrix[1 * 4 + i] * other._matrix[j * 4 + 1] +
											_matrix[2 * 4 + i] * other._matrix[j * 4 + 2] +
											_matrix[3 * 4 + i] * other._matrix[j * 4 + 3];
			}
		}
		return ret;
	}

	inline Vector4f operator*(const Vector4f& other) const {
		Vector4f ret = Vector4f::ZERO;

		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				ret[i] += at(j, i) * (other[j]);
			}
		}
		return ret;
	}

	inline bool operator== (const Matrix4f& other) const {
		for (unsigned int i = 0; i < 16; i++) {
			if (_matrix[i] != other._matrix[i]) return false;
		}
		return true;
	}

	float &operator ()(int x, int y) {
		return _matrix[y + 4 * x];
	}

	friend std::ostream& operator<<(std::ostream& os, Matrix4f m) {
		for (size_t i = 0; i < 4; ++i) {
			os << "[";
			for (size_t j = 0; j < 4; ++j) {
				int f = m.at(j, i);
				os << m.at(j, i);
				if (j < 3)
					os << ",\t";
			}
			os << "]";
			if(i < 3)
				os << std::endl;
		}
		return os;
	}
	static Matrix4f createIdentity();
private:
	float _matrix[16] = {0};

};

#endif // MATRIX4F_H