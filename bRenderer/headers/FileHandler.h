#ifndef B_FILE_HANDLER_H
#define B_FILE_HANDLER_H

#include "OSdetect.h"
#include "Renderer_GL.h"
#include <string>

namespace bRenderer
{
	/**	@brief On desktop systems the standard lookup path for files can be specified
	*	@param[in] path
	*/
	void setStandardFilePath(const std::string &path);

	/**	@brief Get the full path to a file
	*	@param[in] fileName
	*/
	std::string getFilePath(const std::string &fileName);

	/**	@brief Check if a file exists
	 *	@param[in] fileName
	 */
	bool fileExists(const std::string &fileName);

} // namespace bRenderer

#endif /* defined(B_FILE_HANDLER_H) */
