#ifndef BLOOM_PROCESSOR_H
#define BLOOM_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/BlurHShader.h"
#include "../Game/BlurVShader.h"
#include "../Game/CutOffShader.h"
#include "../Game/AddShader.h"

class BloomProcessor : public PostProcessor {
public:
	Material* m_cutOff;
	Material* m_hBlur1;
	Material* m_vBlur1;
	Material* m_hBlur2;
	Material* m_vBlur2;
	//Material* m_add;

	FrameBuffer* fb_original;
	FrameBuffer* fb_cutOff;
	FrameBuffer* fb_blur1;
	FrameBuffer* fb_blur2;
	FrameBuffer* fb_blur3;
	FrameBuffer* fb_blur4;

	BloomProcessor(int index) : PostProcessor(index) {
		m_cutOff = new Material(CutOffShader::getInstance());
		m_cutOff->setProperty("threshold", 1.0f);
		
		m_hBlur1 = new Material(BlurHShader::getInstance());
		m_hBlur1->setProperty("offset", 1.0f/ Display::getWidth() * 15);
		m_vBlur1 = new Material(BlurVShader::getInstance());
		m_vBlur1->setProperty("offset", 1.0f/ Display::getHeight() * 15);

		m_hBlur2 = new Material(BlurHShader::getInstance());
		m_hBlur2->setProperty("offset", 1.0f / Display::getWidth() * 5);
		m_vBlur2 = new Material(BlurVShader::getInstance());
		m_vBlur2->setProperty("offset", 1.0f / Display::getHeight() * 5);

				
		//m_add = new Material(AddShader::getInstance());

		fb_original = new FrameBuffer(Display::getWidth(), Display::getHeight());
		fb_cutOff = new FrameBuffer(Display::getWidth(), Display::getHeight());
		fb_blur1 = new FrameBuffer(Display::getWidth() / 2, Display::getHeight() / 2);
		fb_blur2 = new FrameBuffer(Display::getWidth() / 2, Display::getHeight() / 2);
		fb_blur3 = new FrameBuffer(Display::getWidth() / 8, Display::getHeight() / 8);
		fb_blur4 = new FrameBuffer(Display::getWidth() / 8, Display::getHeight() / 8);
	}

	void enable() override {
		fb_original->enable();
	}

	void preProcess() override{
		fb_cutOff->enable();
		fb_original->render(m_cutOff);

		fb_blur1->enable();
		fb_cutOff->render(m_hBlur1);

		fb_blur2->enable();
		fb_blur1->render(m_vBlur1);
		
		fb_blur3->enable();
		fb_blur2->render(m_hBlur2);
		
		fb_blur4->enable();
		fb_blur3->render(m_vBlur2);
	}

	void render() override {
		fb_original->render();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);
		fb_blur4->render();
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}

	void destroy() override {
		delete m_cutOff;
		//delete m_add;
		delete m_hBlur1;
		delete m_vBlur1;

		fb_original->destroy();
		fb_cutOff->destroy();
		fb_blur1->destroy();
		fb_blur2->destroy();

		delete fb_original;
		delete fb_cutOff;
		delete fb_blur1;
		delete fb_blur2;
	}

};

#endif // BLOOM_PROCESSOR_H