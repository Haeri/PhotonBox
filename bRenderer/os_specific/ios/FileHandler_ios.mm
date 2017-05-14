#include "headers/OSdetect.h"
#ifdef B_OS_IOS

#include "headers/FileHandler.h"
#include "headers/Logger.h"

namespace bRenderer
{
	
	void setStandardFilePath(const std::string &path)
	{
		log("Standard file path cannot be set on iOS", LM_WARNING);
	}

	std::string getFilePath(const std::string &fileName)
    {
        NSString *fullFile = [NSString stringWithCString:fileName.c_str()
                                                   encoding:[NSString defaultCStringEncoding]];
        
        NSString* path = [[fullFile lastPathComponent] stringByDeletingPathExtension];
        NSString* extension = [fullFile pathExtension];
        
        NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:path ofType:extension]];
        
        NSString *urlString = [url path];
        
        return std::string([urlString UTF8String]);
    }
    
    bool fileExists(const std::string &fileName)
    {
        NSString *fullFile = [NSString stringWithCString:fileName.c_str()
                                                encoding:[NSString defaultCStringEncoding]];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: fullFile];
        
        return [fileManager fileExistsAtPath:path];
    }

} // namespace bRenderer

#endif