#include "headers/TextSprite.h"
#include "headers/Logger.h"
#include "headers/FileHandler.h"
#include "headers/ObjectManager.h"

/* Public Functions */

TextSprite::TextSprite(ObjectManager *o, const std::string &name, vmml::Vector3f color, const std::string &text, FontPtr font, PropertiesPtr properties)
	: Sprite(), _text(text), _font(font)
{
	// Create geometry
	createGeometry();

	// Create shader and material
	ShaderPtr shader = o->generateShader(name, { 0, false, true, false, false, true, false, false, false, false, false, false, true });
	MaterialPtr material = o->createMaterial(name, shader);
	
	// Add atlas texture to the material
	material->setTexture(bRenderer::DEFAULT_SHADER_UNIFORM_CHARACTER_MAP(), font->getAtlas());
	// Pass color to material (don't pass texture, has to be treated separately to bind right tex unit)
	material->setVector(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR(), color);

	setMaterial(material);
	setProperties(properties);
}

TextSprite::TextSprite(MaterialPtr material, const std::string &text, FontPtr font, PropertiesPtr	properties)
	: Sprite(), _text(text), _font(font)
{
	// Create geometry
	createGeometry();

	// Add atlas texture to the material
	material->setTexture(bRenderer::DEFAULT_SHADER_UNIFORM_CHARACTER_MAP(), font->getAtlas());

	setMaterial(material);
	setProperties(properties);
}

/* Private Functions */

void TextSprite::createGeometry()
{
	Model::GroupMap &groups = getGroups();
	GeometryPtr &g = groups[bRenderer::DEFAULT_GROUP_NAME()];
	if (!g)
		g = GeometryPtr(new Geometry);

	GeometryDataPtr gData = GeometryDataPtr(new GeometryData);

	GLuint verNum = 6;
	GLuint fontPixelSize = _font->getPixelSize();
	// Initialize position of the first sprite
	GLfloat pos_x = 0.f;
	GLfloat pos_y = 0.f;
	GLfloat z = 0.f;
	bool beginningofLine = true;

	// Create a sprite per character
	for (GLuint i = 0; i < _text.length(); i++){
		if (_text.at(i) == '\n')
		{
			beginningofLine = true;
			pos_x = 0.f;
			pos_y -= 1.0f;
			// The line break has to be erased instead of just increasing i
			// otherwise the indices wouldn't be correct anymore
			_text.erase(_text.begin() + i);
			// If the line break was at the end of the string we have to leave the loop
			if (i >= _text.length())
				break;
		}

		// load character
		ftgl::texture_glyph_t* glyph = _font->getCharacter(_text.at(i));

		if (glyph != NULL)
		{

			GLfloat u0 = glyph->s0;
			GLfloat v0 = glyph->t0;
			GLfloat u1 = glyph->s1;
			GLfloat v1 = glyph->t1;

			GLfloat x0 = pos_x + (beginningofLine ? 0.f : (static_cast<GLfloat>(glyph->offset_x) / fontPixelSize));
			GLfloat y0 = pos_y + (static_cast<GLfloat>(glyph->offset_y) / fontPixelSize);
			GLfloat x1 = x0 + (static_cast<GLfloat>(glyph->width) / fontPixelSize);
			GLfloat y1 = y0 - (static_cast<GLfloat>(glyph->height) / fontPixelSize);

			pos_x += static_cast<GLfloat>(glyph->advance_x) / fontPixelSize;
			pos_y += static_cast<GLfloat>(glyph->advance_y) / fontPixelSize;

			beginningofLine = false;

			// Add vertices
			gData->vboVertices.push_back(Vertex(
				x0, y0, z,					// position
				0.0f, 0.0f, -1.0f,			// normal
				-1.0f, 0.0f, 0.0f,			// tangent
				0.0f, 1.0f, 0.0f,			// bitangent
				u0, v0						// texCoord
			));
			gData->vboVertices.push_back(Vertex(
				x1, y1, z,					// position
				0.0f, 0.0f, -1.0f,			// normal
				-1.0f, 0.0f, 0.0f,			// tangent
				0.0f, 1.0f, 0.0f,			// bitangent
				u1, v1						// texCoord
				));
			gData->vboVertices.push_back(Vertex(
				x1, y0, z,					// position
				0.0f, 0.0f, -1.0f,			// normal
				-1.0f, 0.0f, 0.0f,			// tangent
				0.0f, 1.0f, 0.0f,			// bitangent
				u1, v0						// texCoord
				));
			gData->vboVertices.push_back(Vertex(
				x0, y0, z,					// position
				0.0f, 0.0f, -1.0f,			// normal
				-1.0f, 0.0f, 0.0f,			// tangent
				0.0f, 1.0f, 0.0f,			// bitangent
				u0, v0						// texCoord
				));
			gData->vboVertices.push_back(Vertex(
				x0, y1, z, 					// position
				0.0f, 0.0f, -1.0f,			// normal
				-1.0f, 0.0f, 0.0f,			// tangent
				0.0f, 1.0f, 0.0f,			// bitangent
				u0, v1						// texCoord
				));
			gData->vboVertices.push_back(Vertex(
				x1, y1, z,					// position
				0.0f, 0.0f, -1.0f,			// normal
				-1.0f, 0.0f, 0.0f,			// tangent
				0.0f, 1.0f, 0.0f,			// bitangent
				u1, v1						// texCoord	
				));

			// Add indices
			for (GLushort j = (0 + i*verNum); j < (6 + i*verNum); j++)
				gData->vboIndices.push_back(j);

			// Add index data
			gData->indices.push_back(IndexData(3 + i*verNum, 2 + i*verNum, 0 + i*verNum));
			gData->indices.push_back(IndexData(2 + i*verNum, 1 + i*verNum, 0 + i*verNum));
			gData->indices.push_back(IndexData(0 + i*verNum, 0 + i*verNum, 0 + i*verNum));
			gData->indices.push_back(IndexData(1 + i*verNum, 5 + i*verNum, 0 + i*verNum));
			gData->indices.push_back(IndexData(3 + i*verNum, 4 + i*verNum, 0 + i*verNum));
			gData->indices.push_back(IndexData(0 + i*verNum, 3 + i*verNum, 0 + i*verNum));
		}
	}

	g->initialize(gData);

	setBoundingBoxObjectSpace(g->getBoundingBoxObjectSpace());
}