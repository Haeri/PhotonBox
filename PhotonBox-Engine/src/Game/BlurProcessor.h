#ifndef BLUR_PROCESSOR_H
#define BLUR_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/BlurHShader.h"
#include "../Game/BlurVShader.h"
#include "../Game/CutOffShader.h"
#include "../Game/AddShader.h"

class BlurProcessor : public PostProcessor {
public:
	Material* m_hBlur1;
	Material* m_vBlur1;
	//Material* m_hBlur2;
	//Material* m_vBlur2;

	FrameBuffer* fb_blur1;
	FrameBuffer* fb_blur2;
	//FrameBuffer* fb_blur3;
	//FrameBuffer* fb_blur4;

	BlurProcessor(int index) : PostProcessor(index) {
		m_hBlur1 = new Material(new BlurHShader("./res/post-processing/blur-h"));
		m_hBlur1->setProperty("offset", 0.003f);

		m_vBlur1 = new Material(new BlurVShader("./res/post-processing/blur-v"));
		m_vBlur1->setProperty("offset", 0.003f * (Display::getWidth() / Display::getHeight()));

		fb_blur1 = new FrameBuffer(Display::getWidth()/4, Display::getHeight()/4);
		fb_blur2 = new FrameBuffer(Display::getWidth()/4, Display::getHeight()/4);
	}

	void enable() override {
		fb_blur1->enable();
	}

	void preProcess() override {
		fb_blur2->enable();
		fb_blur1->render(m_hBlur1);
	}

	void render() override {
		fb_blur2->render(m_vBlur1);
	}

	void destroy() override {
		delete m_hBlur1;
		delete m_vBlur1;

		delete fb_blur1;
		delete fb_blur2;
	}

};

#endif // BLUR_PROCESSOR_H