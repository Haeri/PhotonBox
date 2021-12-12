#ifndef OPENGL_H
#define OPENGL_H

#ifdef _WINDOWS
	#define NOMINMAX
	#include <windows.h> // Appentry removes the warning caused my glfw3.h
#endif

#define GLAD_GL_IMPLEMENTATION
#include "../src/backend/opengl/glad/gl.h"
#include <GLFW/glfw3.h>

#endif // OPENGL_H
