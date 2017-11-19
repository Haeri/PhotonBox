#include "CubeMap.h"
#include <vector>
#include <iostream>
#include "STB/stb_image.h"
#include "../Math/Matrix4f.h"
#include "../Resources/IrradianceShader.h"
#include "../Resources/SpecularConvolutionShader.h"
#include "../Resources/SkyBoxShader.h"
#include "OBJLoader.h"
#include "../Components/Camera.h"
#include "../Core/Systems/Renderer.h"

CubeMap::CubeMap(const std::vector<std::string>& allFaces){
	for (size_t lod = 0; lod < allFaces.size(); lod+=6) {
		std::vector<std::string> faces;
		for (size_t i = lod; i < lod + 6; ++i) {
			faces.push_back(allFaces[i]);
		}
		loadCubeMap(faces);
	}
}

CubeMap::CubeMap(int resolutoon): CubeMap(resolutoon, false) {}

CubeMap::CubeMap(int resolutoon, bool mipMaps) {
	_width = resolutoon;
	_height = resolutoon;
	_isMip = mipMaps;

	glGenTextures(1, &_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB16F, resolutoon, resolutoon, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (mipMaps){
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}else {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void CubeMap::generateIrradiance(GLuint map) {
	Vector3f position = Vector3f::ZERO;
	Matrix4f captureProjection = Matrix4f::createPerspective(90.0f, 1.0f, 0.01f, 10.0f);
	Matrix4f captureViews[] =
	{
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(1.0f,  0.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(-1.0f,  0.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f,  1.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f))
	};

	_mesh = OBJLoader::loadObj("./res/primitives/skyBox.obj");
	genVAO();


	glGenFramebuffers(1, &_captureFBO);
	glGenRenderbuffers(1, &_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _captureRBO);


	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);

	IrradianceShader* shader = IrradianceShader::getInstance();
	shader->bind();
	shader->updateTextures();
	shader->setUniform<Matrix4f>("projection", captureProjection);

	for (unsigned int i = 0; i < 6; ++i){
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _cubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		glBindVertexArray(_vao);
		glDepthMask(GL_FALSE);
		shader->setUniform<Matrix4f>("view", captureViews[i]);
		shader->enableAttributes();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, map);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shader->disableAttributes();

		glDepthMask(GL_TRUE);
		glBindVertexArray(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeMap::generateSpecularConvolution(GLuint map) {
	if (!_isMip) {
		std::cerr << "ERROR: Cubemap must have mipmaps to generate Specular convolution" << std::endl;
	}

	Vector3f position = Vector3f::ZERO;
	Matrix4f captureProjection = Matrix4f::createPerspective(90.0f, 1.0f, 0.01f, 10.0f);
	Matrix4f captureViews[] =
	{
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(1.0f,  0.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(-1.0f,  0.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f,  1.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f))
	};

	_mesh = OBJLoader::loadObj("./res/primitives/skyBox.obj");
	genVAO();

	glGenFramebuffers(1, &_captureFBO);
	glGenRenderbuffers(1, &_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _captureRBO);


	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);

	SpecularConvolutionShader* shader = SpecularConvolutionShader::getInstance();
	shader->bind();
	shader->updateTextures();
	shader->setUniform<Matrix4f>("projection", captureProjection);
	
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{



		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = _width * std::pow(0.5, mip);
		unsigned int mipHeight = _height * std::pow(0.5, mip);
		//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		shader->setUniform<float>("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _cubeMap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(_vao);
			glDepthMask(GL_FALSE);
			shader->setUniform<Matrix4f>("view", captureViews[i]);
			shader->enableAttributes();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, map);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			shader->disableAttributes();

			glDepthMask(GL_TRUE);
			glBindVertexArray(0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	/*

	IrradianceShader* shader = IrradianceShader::getInstance();
	shader->bind();
	shader->updateTextures();
	shader->setUniform<Matrix4f>("projection", captureProjection);

	for (unsigned int i = 0; i < 6; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _cubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(_vao);
		glDepthMask(GL_FALSE);
		shader->setUniform<Matrix4f>("view", captureViews[i]);
		shader->enableAttributes();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, map);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shader->disableAttributes();

		glDepthMask(GL_TRUE);
		glBindVertexArray(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	*/
}

void CubeMap::loadCubeMap(const std::vector<std::string>& faces) {
	glGenTextures(1, &_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);

	int nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &_width, &_height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	/*
	/// Generate with mipmaps
	GLuint PreFilteredEnvTex;
	glGenTextures(1, &PreFilteredEnvTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PreFilteredEnvTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, 0);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	*/
}

CubeMap::~CubeMap() {
	glDeleteTextures(1, &_cubeMap);
}

/*
void CubeMap::captureScene(int resolution){
	_width = resolution;
	_height = resolution;

	Camera* oldMain = Camera::getMainCamera();
	Camera cam = Camera();
	cam.setProjection(90.0f, 1.0f, 0.01f, 10.0f);
	cam.setMain();

	Vector3f position = Vector3f::ZERO;
	Matrix4f captureProjection = Matrix4f::createPerspective(90.0f, 1.0f, 0.01f, 10.0f);
	Matrix4f captureViews[] =
	{
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(1.0f,  0.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(-1.0f,  0.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f,  0.0f,  1.0f), Vector3f(0.0f,  1.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f,  0.0f, -1.0f), Vector3f(0.0f, -1.0f,  0.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f,  1.0f)),
		Matrix4f::lookAt(position, Vector3f(0.0f, -1.0f,  0.0f), Vector3f(0.0f,  0.0f, -1.0f))
	};

	glGenFramebuffers(1, &_captureFBO);
	glGenRenderbuffers(1, &_captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _captureRBO);


	glViewport(0, 0, resolution, resolution);
	glBindFramebuffer(GL_FRAMEBUFFER, _captureFBO);

	for (unsigned int i = 0; i < 6; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _cubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cam.transform
		Renderer::render();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	oldMain->setMain();
}
*/
void CubeMap::bind() {
	bind(GL_TEXTURE0);
}

void CubeMap::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
}



void CubeMap::genVAO() {
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, _mesh->vertices.size() * sizeof(Vertex), &(_mesh->vertices[0]), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh->indices.size() * sizeof(unsigned int), &(_mesh->indices[0]), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(Vertex::AttibLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(Vertex::AttibLocation::TEXTURECOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void CubeMap::renderCube() {
	glBindVertexArray(_vao);
	glDepthMask(GL_FALSE);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
}