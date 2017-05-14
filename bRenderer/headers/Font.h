#ifndef B_FONT_H
#define B_FONT_H

#include <memory>
#include <string>
#include <map>
#include "Renderer_GL.h"
#include "Texture.h"

#include "external/freetype-gl/freetype-gl.h"

/** @brief A font that can be used to display text
*	@author Benjamin Buergisser
*/
class Font
{
public:

	/* Functions */

	/**	@brief Constructor
	*/
	Font() { _initialized = false; }

	/**	@brief Constructor
	*	@param[in] fileName File name including extension
	*	@param[in] fontPixelSize The nominal  font size in pixels (the characters may vary in size)
	*/
	Font(const std::string &fontFileName, GLuint fontPixelSize);

	/**	@brief Virtual destructor
	*/
	virtual ~Font()	{
        deleteFont();
    }

	/**	@brief Loads a font file
	*	@param[in] fileName File name including extension
	*/
	virtual void loadFont(const std::string &fontFileName){ init(fontFileName, _fontPixelSize); }

	/**	@brief Set the nominal font size in pixels (the characters may vary in size)
	*	@param[in] fontPixelSize The nominal font size in pixels
	*/
	virtual void setPixelSize(GLuint fontPixelSize) { init(_fontFileName, fontPixelSize); }

	/**	@brief Returns the nominal font size in pixels (the characters may vary in size)
	*/
	GLuint getPixelSize() { return _fontPixelSize; }

	/**	@brief Returns the file name of the current font (including extension)
	*/
	const std::string &getFontFileName() { return _fontFileName; }

	/**	@brief Returns a character
	*	@param[in] c The character to get
	*/
	virtual ftgl::texture_glyph_t *getCharacter(char c);

	/**	@brief Returns the character atlas
	*/
	TexturePtr getAtlas(){ return _atlasTexture; }

	/**	@brief Delete the font and its texture atlas
	*/
	virtual void deleteFont();

private:

	/* Functions */

	void init(const std::string &fontFileName, GLuint fontPixelSize);

	/* Variables */

	bool _initialized = false;
	ftgl::texture_atlas_t *_atlas = nullptr;
	ftgl::texture_font_t *_font = nullptr;
	TexturePtr _atlasTexture = nullptr;
	std::string	_fontFileName;
	GLuint _fontPixelSize;
};

typedef std::shared_ptr< Font >  FontPtr;

#endif /* defined(B_FONT_H) */
