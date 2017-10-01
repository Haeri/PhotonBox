#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>
#include "Shader.h"
#include <array>
#include <memory>
#include <type_traits>
#include <iostream>
#include <iomanip>

using byte = unsigned char;

class Material {
public:
	Shader* shader;

	Material(Shader* shader): shader(shader) {}

	// int
	void setUniform(const std::string& uniformName, int value);
	// float
	void setUniform(const std::string& uniformName, float value);
	// bool
	void setUniform(const std::string& uniformName, bool value);
	// Vector2f
	void setUniform(const std::string& uniformName, Vector2f value);
	// Vector3f
	void setUniform(const std::string& uniformName, Vector3f value);
	// Vector4f
	void setUniform(const std::string& uniformName, Vector4f value);
	// Matrix4f
	void setUniform(const std::string& uniformName, Matrix4f value);

	// Texture
	void setTexture(const std::string& uniformName, Texture* texture);
	// CubeMap
	void setCubeMap(const std::string& uniformName, CubeMap* cubeMap);
	

	/*
	template <typename T>
	T getUniform(const std::string& uniformName) {
		return _uniformMap[uniformName].getValue();
	}
	*/

	void updateUniforms();
	void updateUniforms(Shader* shader);
	void bindTextures();
	void bindTextures(Shader* customShader);

	Texture* getTexture(const std::string& uniformName);
	CubeMap* getCubeMap(const std::string& uniformName);
private:
	
	struct BaseObject{
		std::string _name;
		virtual void update(Shader* shader) = 0;
		
	};

	struct IntObject : BaseObject { 
		int value; 
		IntObject(int value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//int getValue() override { return value; }
	};
	struct FloatObject : BaseObject { 
		float value; 
		FloatObject(float value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//float getValue() override { return value; }
	};
	struct BoolObject : BaseObject { 
		bool value; 
		BoolObject(bool value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//bool getValue() override { return value; }
	};
	struct Vec2Object : BaseObject { 
		Vector2f value; 
		Vec2Object(Vector2f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//Vector2f getValue() override { return value; }
	};
	struct Vec3Object : BaseObject {
		Vector3f value; 
		Vec3Object(Vector3f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//Vector3f getValue() override { return value; }
	};
	struct Vec4Object : BaseObject {
		Vector4f value; 
		Vec4Object(Vector4f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//Vector4f getValue() override { return value; }
	};
	struct Mat4Object : BaseObject {
		Matrix4f value; 
		Mat4Object(Matrix4f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		//Matrix4f getValue() override { return value; }
	};
	

	std::unordered_map<std::string, Texture*> _textreMap;
	std::unordered_map<std::string, CubeMap*> _cubeMapMap;
	std::unordered_map<std::string, BaseObject*> _uniformMap;
};

#endif /* defined(MATERIAL_H) */