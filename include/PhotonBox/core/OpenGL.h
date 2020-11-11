#ifndef OPENGL_H
#define OPENGL_H

#ifdef _WINDOWS
	#define NOMINMAX
	#include <windows.h> // Removes the appentry warning caused my glfw3.h
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#endif // OPENGL_H
