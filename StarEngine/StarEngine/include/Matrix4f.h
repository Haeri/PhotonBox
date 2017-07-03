#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "Vector3f.h"

class Matrix4f {
public:
	const static Matrix4f IDENTITY;

	static Matrix4f createRotation(float angle, Vector3f axis);
	static Matrix4f createScaling(float x, float y, float z);
	static Matrix4f createScaling(Vector3f vec);
	static Matrix4f createTranslation(float x, float y, float z);
	static Matrix4f createTranslation(Vector3f vec);
	
	const float at(unsigned int  x, unsigned int  y) const;
	void set(unsigned int x, unsigned int  y, float val);

	inline Matrix4f operator* (const Matrix4f& other) const {
		Matrix4f ret;
		for (unsigned int i = 0; i < 4; i++)
		{
			for (unsigned int j = 0; j < 4; j++)
			{
				ret.set(i, j, 0);
				for (unsigned int k = 0; k < 4; k++)
					ret.set(i, j, ret.at(i, j) + at(k, j) * other.at(i, k));
			}
		}
		return ret;
	}


	float &operator ()(int x, int y) {
		return _matrix[x  + 4 * y];
	}

private:
	float _matrix[16] = {0};

	static Matrix4f createIdentity();
};

#endif /* defined(MATRIX4F_H) */