#ifndef B_TEXT_SPRITE_H
#define B_TEXT_SPRITE_H

#include "Sprite.h"
#include "Configuration.h"
#include "Font.h"

/** @brief A simple text sprite to display 2d text on the screen
*	@author Benjamin Buergisser
*/
class TextSprite : public Sprite
{
public:

	/* Functions */

	/**	@brief Constructor
	*	@param[in] o Object management
	*	@param[in] name The raw name of the sprite
	*	@param[in] color The color of the text
	*	@param[in] text The string to be displayed
	*	@param[in] font The font to be used to display the string
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	TextSprite(ObjectManager *o, const std::string &name, vmml::Vector3f color, const std::string &text, FontPtr font, PropertiesPtr properties = nullptr);
	
	/**	@brief Constructor
	*	@param[in] material
	*	@param[in] text The string to be displayed
	*	@param[in] font The font to be used to display the string
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	TextSprite(MaterialPtr material, const std::string &text, FontPtr font, PropertiesPtr properties = nullptr);

	/**	@brief Virtual destructor
	*/
	virtual ~TextSprite() { deleteModelGeometry(); }

	/**	@brief Set a text string
	*	@param[in] text The string to be displayed
	*/
	void setText(const std::string &text) { _text = text; createGeometry(); }

	/**	@brief Returns the text string of the sprite
	*/
	std::string getText() { return _text; }

	/**	@brief Set the font to be used to display the string 
	*	@param[in] font The font to be used to display the string
	*/
	void setFont(FontPtr font) { _font = font; createGeometry(); }

	/**	@brief Returns the font used to display the string
	*/
	FontPtr getFont() { return _font; }

	/**	@brief Sets a color for the text
	*	@param[in] color The color for the text
	*/
	void setColor(const vmml::Vector3f &color) { getMaterial()->setVector(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR(), color); }

private:

	/* Functions */
	//void draw(GLenum mode, bool drawInstance, const std::string &instanceName);

	void createGeometry();

	/* Variables */
	
	FontPtr _font;
	std::string	_text;
};

typedef std::shared_ptr< TextSprite >  TextSpritePtr;

#endif /* defined(B_TEXT_SPRITE_H) */
