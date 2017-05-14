#ifndef B_LOGGER_H
#define B_LOGGER_H

#include "Renderer_GL.h"
#include <iostream>
#include <string>

namespace bRenderer
{
	enum LogMode
	{
		LM_INFO,
		LM_WARNING,
		LM_ERROR,
		LM_SYS
	};

	/**	@brief Write a string to the console
	 *	@param[in] msg The message to be displayed
	 *	@param[in] mode Defines the mode of the logger (adds a tag to the string)
	 */
	void log(const std::string &msg, LogMode mode = LM_INFO);

	/**	@brief Write an OpenGL string to the console
	 *	@param[in] arg The OpenGL string to be displayed
	 *	@param[in] mode Defines the mode of the logger (adds a tag to the string)
	 */
	void log(const GLubyte *arg, LogMode mode = LM_INFO);

	/**	@brief Write a message and an OpenGL string to the console
	 *	@param[in] msg The message to be displayed
	 *	@param[in] arg The OpenGL string to be displayed
	 *	@param[in] mode Defines the mode of the logger (adds a tag to the string)
	 */
	void log(const std::string &msg, const GLubyte *arg, LogMode mode = LM_INFO);

} // namespace bRenderer

#endif /* defined(B_LOGGER_H) */
