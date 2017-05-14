#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vmmlib/util.hpp>
#include "Component.h"

class Transform : public Component{
public:
	vmml::Vector3f position = vmml::Vector3f::ZERO;
	vmml::Vector4f rotation = vmml::Vector4f(0, 0, 0, 1);
	vmml::Vector3f scale = vmml::Vector3f::ONE;

	void setParent(Transform *_parent);
	void setParent(GameObject *_gameObject);
	vmml::Matrix4f getTransformationMatrix();
private:
	Transform *parent;
	vmml::Matrix4f getLocalTransformationMatrix();
};
#endif /* defined(TRANSFORM_H) */
