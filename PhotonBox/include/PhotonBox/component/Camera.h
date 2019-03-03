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
	};

	Camera();

	void updateAspect();
	void updateProjection();
	void setFOV(float fov);
	void setOrhographicProjection(float radius, float zNear, float zFar);
	void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
	void setMain();
	bool frustumTest(ObjectRenderer* object);

	Matrix4f getProjectionMatrix();
	Matrix4f getViewMatrix();
	Matrix4f getViewProjection();
	Matrix4f getViewProjectionJittered();
	Vector4f getProjectionExtents(float xOffset = 0, float yOffset = 0);
	float getFOV() { return _fov; }

	// TODO: This is ugly. Make this pretty
	void storeOldVP();
	Matrix4f _viewProjectionLast;

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
	int _count = 0;

	Vector2f _hilton16[16] = {
		Vector2f(0.00398f, 0.99203f),
		Vector2f(0.49402f, 0.66002f),
		Vector2f(0.24501f, 0.32802f),
		Vector2f(0.74302f, 0.88136f),
		Vector2f(0.12051f, 0.54935f),
		Vector2f(0.61852f, 0.21735f),
		Vector2f(0.36952f, 0.77069f),
		Vector2f(0.86752f, 0.43868f),
		Vector2f(0.05826f, 0.10668f),
		Vector2f(0.55627f, 0.95514f),
		Vector2f(0.30727f, 0.62313f),
		Vector2f(0.80527f, 0.29113f),
		Vector2f(0.18276f, 0.84447f),
		Vector2f(0.68077f, 0.51246f),
		Vector2f(0.43177f, 0.18046f),
		Vector2f(0.92978f, 0.73380f),
	};


	void updateFrustum();
};

#endif // CAMERA_H
