#ifndef UI_RENDERER_H
#define UI_RENDERER_H

class TextShader;

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include "../../Math/Vector2f.h"
#include "../../Math/Vector3f.h"
#include "../../Core/OpenGL.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	Vector2f Size;    // Size of glyph
	Vector2f Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class UIRenderer {
public:
	void init();
	static void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, Vector3f color);
private:
	static TextShader* shader;
	static GLuint _VAO, _VBO;
	static std::map<GLchar, Character> _characters;
};

#endif // UI_RENDERER_H