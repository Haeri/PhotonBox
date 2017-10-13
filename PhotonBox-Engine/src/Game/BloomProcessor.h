#ifndef BLOOM_PROCESSOR_H
#define BLOOM_PROCESSOR_H

#include "../Core/PostProcessor.h"
#include "../Game/BlurSHader.h"
#include "../Game/CutOffShader.h"
#include "../Game/AddShader.h"

class BloomProcessor : public PostProcessor {
public:
	Material* m_cutOff;
	Material* m_blur;
	Material* m_add;

	FrameBuffer* fb_original;
	FrameBuffer* fb_cutOff;
	FrameBuffer* fb_blur;

	BloomProcessor(int index) : PostProcessor(index) {
		m_cutOff = new Material(new CutOffShader("./res/post-processing/cutOff"));
		m_cutOff->setProperty<float>("threshold", 0.5f);
		m_blur = new Material(new BlurShader("./res/post-processing/blur"));
		m_blur->setProperty("aspectRatio", (float)(Display::getWidth() / Display::getHeight()));
		m_blur->setProperty("offset", 0.01f);
		m_add = new Material(new AddShader("./res/post-processing/add"));

		fb_original = new FrameBuffer(Display::getWidth(), Display::getHeight());
		fb_cutOff = new FrameBuffer(Display::getWidth(), Display::getHeight());
		fb_blur = new FrameBuffer(Display::getWidth(), Display::getHeight());
	}

	void enable() override {
		fb_original->enable();
	}

	void preProcess() override{
		fb_cutOff->enable();
		fb_original->render(m_cutOff);
		fb_blur->enable();
		fb_cutOff->render(m_blur);
	}

	void render() override {		
		fb_original->render(m_add, fb_blur->getTextureID());
	}

	void destroy() override {
		delete m_cutOff;
		delete m_blur;
		delete m_add;

		delete fb_original;
		delete fb_cutOff;
		delete fb_blur;
	}

};

#endif // BLOOM_PROCESSOR_H