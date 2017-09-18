#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class BasicShader: public Shader {
public:
	BasicShader(const std::string& fileName) { init(fileName); }

	void addUniforms() override {
		addUniform("transform");
	}
	
	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
		addAttribut("normal", Vertex::AttibLocation::NORMAL);
		addAttribut("color", Vertex::AttibLocation::COLOR);
		addAttribut("uv", Vertex::AttibLocation::TEXTURECOORD);
	}
};

#endif // BASIC_SHADER_H
