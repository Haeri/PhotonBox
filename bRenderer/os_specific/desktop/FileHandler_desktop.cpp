#include "headers/OSdetect.h"

#ifdef B_OS_DESKTOP

#include "headers/FileHandler.h"
#include "headers/Logger.h"
#include "headers/Configuration.h"
#include <sys/stat.h>

namespace bRenderer
{
	/* Internal variables */
	
	std::string standardFilePath = bRenderer::DEFAULT_FILE_PATH();

	/* External functions */

	void setStandardFilePath(const std::string &path)
	{
		standardFilePath = path;

		if (path.find_last_of("/") != path.length()-1)
			standardFilePath.append("/");
	}

	std::string getFilePath(const std::string &fileName)
	{	
		std::string filePath = standardFilePath;
		filePath.append(fileName);

		if (!fileExists(filePath))
		{
			log("File not found: " + fileName, LM_ERROR);
		}

		return filePath;
	}

	bool fileExists(const std::string &fileName)
	{
		struct stat buffer;

		if (fileName.find(standardFilePath) == std::string::npos){
			std::string filePath = standardFilePath;
			filePath.append(fileName);
			return (stat(filePath.c_str(), &buffer) == 0);
		}	
		else {
			return (stat(fileName.c_str(), &buffer) == 0);
		}
		
	}

} // namespace bRenderer

#endif