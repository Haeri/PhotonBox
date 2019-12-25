#include "PhotonBox/util/GLError.h"

#include <iostream>
#include <string>

#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/core/system/ResourceManager.h"
#include "PhotonBox/util/Logger.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void _check_gl_error(const char *file, int line)
{
#ifdef PB_DEBUG
	//TODO: This check should be disabled when resource loading is implemented
	// Correctly so there is no need for this
	if (!ResourceManager::isCompleted()) return;

	GLenum err = glGetError();

	while (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}


		Logger::logn("GL_" + error + " - " + file + ":" + std::to_string(line), Logger::ERR);
		err = glGetError();
	}
#endif // PB_DEBUG
}
