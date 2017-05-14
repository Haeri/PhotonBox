#include "headers/Font.h"
#include "headers/Logger.h"
#include "headers/FileHandler.h"
#include "headers/Configuration.h"

Font::Font(const std::string &fontFileName, GLuint fontPixelSize)
{
	_initialized = false;
	init(fontFileName, fontPixelSize);
}

ftgl::texture_glyph_t *Font::getCharacter(char c)
{
	return ftgl::texture_font_get_glyph(_font, c);
}

/* Private Functions */

void Font::init(const std::string &fontFileName, GLuint fontPixelSize)
{
	if (fontPixelSize > bRenderer::FONT_MAX_PIXEL_SIZE())
		fontPixelSize = bRenderer::FONT_MAX_PIXEL_SIZE();
	if (_fontFileName != fontFileName || _fontPixelSize != fontPixelSize){
		if (_initialized)
			deleteFont();		// If the font has been initialized before everything has to be reset

		_atlas = ftgl::texture_atlas_new(8 * fontPixelSize, 8 * fontPixelSize, 1);
		_font = ftgl::texture_font_new_from_file(_atlas, fontPixelSize, bRenderer::getFilePath(fontFileName).c_str());

		// Load character to initialize atlas (otherwise id is 0)
		ftgl::texture_font_get_glyph(_font, 'b');

		_atlasTexture = TexturePtr(new Texture(_atlas->id));
	}
	_fontFileName = fontFileName;
	_fontPixelSize = fontPixelSize;
	_initialized = true;
}

void Font::deleteFont()
{
	if (_atlas)
		ftgl::texture_atlas_delete(_atlas);
	if (_font)
		ftgl::texture_font_delete(_font);
	if (_atlasTexture)
		_atlasTexture->deleteTexture();

	_atlas = nullptr;
	_font = nullptr;
	_atlasTexture = nullptr;
}