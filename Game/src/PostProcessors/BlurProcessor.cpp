#ifndef BLUR_PROCESSOR_CPP
#define BLUR_PROCESSOR_CPP

#include <Core/PostProcessor.h>

#include "../Shader/AddShader.cpp"
#include "../Shader/BlurHShader.cpp"
#include "../Shader/BlurVShader.cpp"
#include "../Shader/CutOffShader.cpp"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class BlurProcessor : public PostProcessor
{
public:
	Material * m_hBlur1;
	Material* m_vBlur1;
	Material* m_hBlur2;
	Material* m_vBlur2;

	FrameBuffer* fb_blur1;
	FrameBuffer* fb_blur2;
	FrameBuffer* fb_blur3;
	FrameBuffer* fb_blur4;

	BlurProcessor(int index) : PostProcessor(index)
	{
		m_hBlur1 = new Material(BlurHShader::getInstance());
		m_hBlur1->setProperty("offset", 0.005f);
		m_vBlur1 = new Material(BlurVShader::getInstance());
		m_vBlur1->setProperty("offset", 0.005f * (Display::getWidth() / Display::getHeight()));

		m_hBlur2 = new Material(BlurHShader::getInstance());
		m_hBlur2->setProperty("offset", 0.005f);
		m_vBlur2 = new Material(BlurVShader::getInstance());
		m_vBlur2->setProperty("offset", 0.005f * (Display::getWidth() / Display::getHeight()));

		fb_blur1 = new FrameBuffer(Display::getWidth() / 2, Display::getHeight() / 2);
		fb_blur1->addTextureAttachment("color");
		fb_blur1->ready();
		fb_blur2 = new FrameBuffer(Display::getWidth() / 2, Display::getHeight() / 2);
		fb_blur2->addTextureAttachment("color");
		fb_blur2->ready();
		fb_blur3 = new FrameBuffer(Display::getWidth() / 8, Display::getHeight() / 8);
		fb_blur3->addTextureAttachment("color");
		fb_blur3->ready();
		fb_blur4 = new FrameBuffer(Display::getWidth() / 8, Display::getHeight() / 8);
		fb_blur4->addTextureAttachment("color");
		fb_blur4->ready();
	}

	void prepare() override
	{
		fb_blur1->enable();
	}

	void preProcess() override
	{
		fb_blur2->enable();
		fb_blur1->render(m_hBlur1);
		fb_blur3->enable();
		fb_blur2->render(m_vBlur1);
		fb_blur4->enable();
		fb_blur3->render(m_hBlur2);
	}

	void render() override
	{
		fb_blur4->render(m_vBlur2);
	}

	void destroy() override
	{
		delete m_hBlur1;
		delete m_vBlur1;
		delete m_hBlur2;
		delete m_vBlur2;

		delete fb_blur1;
		delete fb_blur2;
		delete fb_blur3;
		delete fb_blur4;
	}

};

#endif // BLUR_PROCESSOR_CPP