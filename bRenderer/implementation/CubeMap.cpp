#include "headers/CubeMap.h"
#include "headers/TextureData.h"

CubeMap::CubeMap(const std::vector<TextureData> &data)
	: Texture()
{
	if (data.size() >= 6){
		glBindTexture(GL_TEXTURE_CUBE_MAP, getTextureID());

		for (GLuint i = 0; i < 6; i++){
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data[i].getFormat(), data[i].getWidth(), data[i].getWidth(), 0, data[i].getFormat(), GL_UNSIGNED_BYTE, data[i].getImageData().get());
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

void CubeMap::bind(GLint texUnit)
{
	glActiveTexture(texUnit);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, getTextureID());
}