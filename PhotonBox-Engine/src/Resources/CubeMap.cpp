#include "CubeMap.h"
#include <vector>
#include <iostream>
#include "STB/stb_image.h"

CubeMap::CubeMap(const std::vector<std::string>& allFaces){
	for (size_t lod = 0; lod < allFaces.size(); lod+=6) {
		std::vector<std::string> faces;
		for (size_t i = lod; i < lod + 6; ++i) {
			faces.push_back(allFaces[i]);
		}
		loadCubeMap(faces);
		++_maxLod;
	}
}

void CubeMap::loadCubeMap(const std::vector<std::string>& faces) {
	GLuint _cubeMap;
	glGenTextures(1, &_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
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



	_cubeMaps.push_back(_cubeMap);
}

CubeMap::~CubeMap() {
	glDeleteTextures(_maxLod, &(_cubeMaps.at(0)));
}

void CubeMap::bind() {
	bind(GL_TEXTURE0, 0);
}

void CubeMap::bind(GLenum textureUnit) {
	bind(textureUnit, 0);
}

void CubeMap::bind(GLenum textureUnit, int lod) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMaps[lod]);
}