#ifndef CAMERA_H
#define CAMERA_H

class ObjectRenderer;

#include "PhotonBox/math/Matrix4f.h"
#include "PhotonBox/components/Transform.h"

#include "PhotonBox/core/Component.h"

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
	int _count;

	Vector2f _hilton16[16] = {
		Vector2f(0.00398, 0.99203),
		Vector2f(0.49402, 0.66002),
		Vector2f(0.24501, 0.32802),
		Vector2f(0.74302, 0.88136),
		Vector2f(0.12051, 0.54935),
		Vector2f(0.61852, 0.21735),
		Vector2f(0.36952, 0.77069),
		Vector2f(0.86752, 0.43868),
		Vector2f(0.05826, 0.10668),
		Vector2f(0.55627, 0.95514),
		Vector2f(0.30727, 0.62313),
		Vector2f(0.80527, 0.29113),
		Vector2f(0.18276, 0.84447),
		Vector2f(0.68077, 0.51246),
		Vector2f(0.43177, 0.18046),
		Vector2f(0.92978, 0.73380),
	};


	void updateFrustum();
};

#endif // CAMERA_H
