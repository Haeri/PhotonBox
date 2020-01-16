#ifndef CAMERA_H
#define CAMERA_H

class ObjectRenderer;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/math/Matrix4f.h"
#include "PhotonBox/component/Transform.h"

class Camera : public Component
{
public:
	struct Plane
	{
		Vector3f normal;
		float distance;

		void normalize()
		{
			float mag = sqrt(normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z());
			normal.x() = normal.x() / mag;
			normal.y() = normal.y() / mag;
			normal.z() = normal.z() / mag;
			distance = distance / mag;
		}
	};

	void updateAspect();
	void updateProjection();
	void setFOV(float fov);
	void setOrhographicProjection(float radius, float zNear, float zFar);
	void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
	void setMain();
	bool frustumTest(ObjectRenderer* object);
	void toggleJitter(bool jitter);

	Matrix4f getProjectionMatrix();
	Matrix4f getViewMatrix();
	Matrix4f getViewProjection();
	Vector4f getProjectionExtents(float xOffset = 0, float yOffset = 0);
	float getFOV() { return _fov; }

	void init() override;
	void destroy() override;

	static Vector2f worldToScreen(Vector3f point);
	static Camera* getMainCamera() { return _main; }
private:
	static Camera* _main;

	unsigned long int _lastFrustumUpdate;
	float _fov, _zNear, _zFar, _aspect, _radius;
	bool _isPerspective;
	bool _jitter = false;
	Matrix4f _projectionMatrix;
	Matrix4f _viewMatrix;
	Plane _frustum[6];

	void updateFrustum();
};

#endif // CAMERA_H
