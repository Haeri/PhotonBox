#ifndef POST_SHADER_H
#define POST_SHADER_H

#include "Shader.h"
#include "Vertex.h"

class PostShader : public InstancedShader<PostShader>{
public:
	std::string filePath = "";

	void addUniforms() override {}

	void addAttributes() override {
		addAttribut("position", Vertex::AttibLocation::POSITION);
	}
};

#endif // POST_SHADER_H
