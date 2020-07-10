#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

class ImageBuffer
{
public:
	virtual void enable() = 0;
	virtual void bind(unsigned int textureUnit) = 0;
};

#endif // IMAGE_BUFFER_H