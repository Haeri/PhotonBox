#ifndef BLUR_PROCESSOR_CPP
#define BLUR_PROCESSOR_CPP

#include <resource/PostProcessor.h>
#include <resource/Material.h>

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

	BlurProcessor(int index) : PostProcessor(index, 0.5f)
	{
		_m_hBlur1 = new Material(BlurHShader::getInstance());
		_m_hBlur1->setProperty("offset", 0.005f);
		_m_vBlur1 = new Material(BlurVShader::getInstance());
		_m_vBlur1->setProperty("offset", 0.005f * (Display::getWidth() / Display::getHeight()));

		_m_hBlur2 = new Material(BlurHShader::getInstance());
		_m_hBlur2->setProperty("offset", 0.005f);
		_m_vBlur2 = new Material(BlurVShader::getInstance());
		_m_vBlur2->setProperty("offset", 0.005f * (Display::getWidth() / Display::getHeight()));

		_fb_blur2 = new FrameBuffer(1/2.0f);
		_fb_blur2->addTextureAttachment("color");
		_fb_blur2->ready();
		_fb_blur3 = new FrameBuffer(1/8.0f);
		_fb_blur3->addTextureAttachment("color");
		_fb_blur3->ready();
		_fb_blur4 = new FrameBuffer(1/8.0f);
		_fb_blur4->addTextureAttachment("color");
		_fb_blur4->ready();
	}

	void render(FrameBuffer* nextBuffer) override
	{
		_fb_blur2->enable();
		mainBuffer->render(_m_hBlur1);
		_fb_blur3->enable();
		_fb_blur2->render(_m_vBlur1);
		_fb_blur4->enable();
		_fb_blur3->render(_m_hBlur2);
		nextBuffer->enable();
		_fb_blur4->render(_m_vBlur2);
	}

	void destroy() override
	{
		delete _m_hBlur1;
		delete _m_vBlur1;
		delete _m_hBlur2;
		delete _m_vBlur2;

		delete _fb_blur2;
		delete _fb_blur3;
		delete _fb_blur4;
	}

private:
	Material* _m_hBlur1;
	Material* _m_vBlur1;
	Material* _m_hBlur2;
	Material* _m_vBlur2;

	FrameBuffer* _fb_blur2;
	FrameBuffer* _fb_blur3;
	FrameBuffer* _fb_blur4;
};

#endif // BLUR_PROCESSOR_CPP