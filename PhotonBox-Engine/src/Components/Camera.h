#ifndef CAMERA_H
#define CAMERA_H

#include "../Core/Component.h"
#include "Transform.h"
#include "../Math/Matrix4f.h"
#include "../Core/Display.h"
#include "../Math/Vector2f.h"

class Camera: public Component{
public:
	Camera();
	void updateAspect();
	void updateProjection();
	void setFOV(float fov);
	void setProjection(float fov, float aspect, float zNear, float zFar);
	void setMain();

	Matrix4f getProjectionMatrix() { return _projection; }
	Matrix4f getViewMatrix();
	Matrix4f getViewProjection();
	float getFOV() { return _fov; }

	void destroy() override;

	static Vector2f worldToScreen(Vector3f point);
	static Camera* getMainCamera() { return _main; }
private:
	static Camera* _main;
	float _fov, _zNear, _zFar, _aspect;
	Matrix4f _projection;
};
#endif // CAMERA_H
