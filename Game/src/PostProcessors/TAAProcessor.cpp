#ifndef TAA_PROCESSOR_CPP
#define TAA_PROCESSOR_CPP

#include <core/system/Renderer.h>
#include <resource/PostProcessor.h>
#include <resource/Material.h>
#include <resource/FrameBuffer.h>

#include "../Shader/TAAShader.cpp"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

class TAAProcessor : public PostProcessor
{
public:

	TAAProcessor(int index) : PostProcessor(index)
	{
		_flipBuf1 = new FrameBuffer(1);
		_flipBuf1->addTextureAttachment("color", true);
		_flipBuf1->ready();

		_flipBuf2 = new FrameBuffer(1);
		_flipBuf2->addTextureAttachment("color", true);
		_flipBuf2->ready();

		_taaShader = new Material(TAAShader::getInstance());
		_taaShader->setImageBuffer("lowResTexture", mainBuffer->getAttachment("color"));
		_taaShader->setImageBuffer("gPosition", Renderer::getGBuffer()->getAttachment("gPosition"));
		_taaShader->setImageBuffer("normalVelocity", Renderer::getGBuffer()->getAttachment("gMetallic"));
	}

	void render(FrameBuffer* nextBuffer) override
	{
		// Prepare
		if (_flip)
		{
			_flipBuf2->enable();
			_taaShader->setImageBuffer("previousLowResTexture", _flipBuf1->getAttachment("color"));
			mainBuffer->render(_taaShader);
		}
		else
		{
			_flipBuf1->enable();
			_taaShader->setImageBuffer("previousLowResTexture", _flipBuf2->getAttachment("color"));
			mainBuffer->render(_taaShader);
		}

		// Render
		nextBuffer->enable();
		if (_flip)
		{
			_flipBuf2->render("color");
		}
		else
		{
			_flipBuf1->render("color");
		}

		_flip = !_flip;
	}

	void onEnableChange(bool enabled) override
	{
		Camera::getMainCamera()->toggleJitter(enabled);
	}

	void destroy() override
	{
		delete _taaShader;
		delete _flipBuf1;
		delete _flipBuf2;
	}

private:
	bool _flip = true;
	Material * _taaShader;
	FrameBuffer* _flipBuf1, *_flipBuf2;
};

#endif // TAA_PROCESSOR_CPP