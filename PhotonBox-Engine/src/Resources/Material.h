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

	template<typename T>
	void Material::setUniform(const std::string & uniformName, T value) {
		_uniformMap[uniformName] = to_bytes(value);
	}

/*
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
	*/

	// Texture
	void setTexture(const std::string& uniformName, Texture* texture);
	// CubeMap
	void setCubeMap(const std::string& uniformName, CubeMap* cubeMap);
	

	template <typename T>
	T getUniform(const std::string& uniformName) {
		return _uniformMap[uniformName].getValue();
	}

	void updateUniforms();
	void updateUniforms(Shader* shader);
	void bindTextures();
	void bindTextures(Shader* customShader);

	Texture* getTexture(const std::string& uniformName);
	CubeMap* getCubeMap(const std::string& uniformName);
private:
	template< typename T >
	std::array< byte, sizeof(T) >  to_bytes(const T& object)
	{
		std::array< byte, sizeof(T) > bytes;

		const byte* begin = reinterpret_cast< const byte* >(std::addressof(object));
		const byte* end = begin + sizeof(T);
		std::copy(begin, end, std::begin(bytes));

		return bytes;
	}

	template< typename T >
	T& from_bytes(const std::array< byte, sizeof(T) >& bytes, T& object)
	{
		// http://en.cppreference.com/w/cpp/types/is_trivially_copyable
		static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

		byte* begin_object = reinterpret_cast< byte* >(std::addressof(object));
		std::copy(std::begin(bytes), std::end(bytes), begin_object);

		return object;
	}

	/*

	template <class T>
	struct BaseObject : SuperBaseObject{ 
		std::string _name;
		virtual void update(Shader* shader) = 0;
		virtual T getValue() = 0;
	};
	struct IntObject : BaseObject<int> { 
		int value; 
		IntObject(int value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		int getValue() override { return value; }
	};
	struct FloatObject : BaseObject<float> { 
		float value; 
		FloatObject(float value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		float getValue() override { return value; }
	};
	struct BoolObject : BaseObject<bool> { 
		bool value; 
		BoolObject(bool value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		bool getValue() override { return value; }
	};
	struct Vec2Object : BaseObject<Vector2f> { 
		Vector2f value; 
		Vec2Object(Vector2f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		Vector2f getValue() override { return value; }
	};
	struct Vec3Object : BaseObject<Vector3f>{
		Vector3f value; 
		Vec3Object(Vector3f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		Vector3f getValue() override { return value; }
	};
	struct Vec4Object : BaseObject<Vector4f>{
		Vector4f value; 
		Vec4Object(Vector4f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		Vector4f getValue() override { return value; }
	};
	struct Mat4Object : BaseObject<Matrix4f>{
		Matrix4f value; 
		Mat4Object(Matrix4f value, Shader* shader, std::string name) : value(value) { _name = name; }
		void update(Shader* shader) override { shader->setUniform(_name, value); }
		Matrix4f getValue() override { return value; }
	};
	*/

	std::unordered_map<std::string, Texture*> _textreMap;
	std::unordered_map<std::string, CubeMap*> _cubeMapMap;
	std::unordered_map<std::string, byte*> _uniformMap;
};

#endif /* defined(MATERIAL_H) */