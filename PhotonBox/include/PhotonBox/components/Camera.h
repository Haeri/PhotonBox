#ifndef CAMERA_H
#define CAMERA_H

class ObjectRenderer;

#include "PhotonBox/core/Component.h"
#include "PhotonBox/core/Display.h"
#include "PhotonBox/math/Matrix4f.h"
#include "PhotonBox/math/Vector2f.h"
#include "PhotonBox/components/Transform.h"

class Camera : public Component
{
public:
	struct Plane
	{
		Vector3f normal;
		float distance;
	};

	Camera();

	void updateAspect();
	void updateProjection();
	void setFOV(float fov);
	void setOrhographicProjection(float radius, float zNear, float zFar);
	void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
	void setMain();
	bool frustumTest(ObjectRenderer* object);

	Matrix4f getProjectionMatrix() { return _projection; }
	Matrix4f getViewMatrix();
	Matrix4f getViewProjection();
	float getFOV() { return _fov; }

	void destroy() override;

	static Vector2f worldToScreen(Vector3f point);
	static Camera* getMainCamera() { return _main; }
private:
	static Camera* _main;
	float _fov, _zNear, _zFar, _aspect, _radius;
	bool _isPerspective;
	Matrix4f _projection;
	Matrix4f _viewCache;
	Plane _frustum[6];
	Transform _lastTransform;

	void updateFrustum();
};

#endif // CAMERA_H
