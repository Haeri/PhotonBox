#include "headers/OSdetect.h"
#ifdef B_OS_IOS

#include "headers/TextureData.h"

TextureData &TextureData::load(const std::string &fileName)
{
    static const size_t COMPONENTS          = 4;
    static const size_t BITS_PER_COMPONENT  = 8;
    
    _format = GL_RGBA;
    
    NSString *fullFileName = [NSString stringWithCString:fileName.c_str()
                                                encoding:[NSString defaultCStringEncoding]];
    
    CGImageRef textureImage = [UIImage imageNamed:fullFileName].CGImage;
    
    if (!textureImage) {
        NSLog(@"Failed to load image %@", fullFileName);
        exit(1);
    }

    _width = CGImageGetWidth(textureImage);
    _height = CGImageGetHeight(textureImage);
    _imageData = ImageDataPtr((GLubyte *) calloc(_width * _height * COMPONENTS, sizeof(GLubyte)), free);
    
    CGContextRef spriteContext = CGBitmapContextCreate(_imageData.get(), _width, _height, BITS_PER_COMPONENT, _width * COMPONENTS,
                                                       CGImageGetColorSpace(textureImage), kCGImageAlphaPremultipliedLast);
    
    // flip vertically
    CGContextTranslateCTM(spriteContext, 0, _height);
    CGContextScaleCTM(spriteContext, 1.0, -1.0);
    
    CGContextDrawImage(spriteContext, CGRectMake(0, 0, _width, _height), textureImage);
    
    CGContextRelease(spriteContext);
    
    return *this;
}

#endif