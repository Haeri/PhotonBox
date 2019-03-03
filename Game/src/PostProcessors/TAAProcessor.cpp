#ifndef TAA_PROCESSOR_CPP
#define TAA_PROCESSOR_CPP

#include <core/PostProcessor.h>
#include <resources/Material.h>
#include <core/systems/Renderer.h>

#include "../Shader/TAAShader.cpp"

#ifdef MEM_DEBUG
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
		_taaShader->setTexture("lowResTexture", mainBuffer, "color");
		_taaShader->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
		_taaShader->setTexture("normalVelocity", Renderer::getGBuffer(), "gMetallic");
	}

	void render(FrameBuffer* nextBuffer) override
	{
		// Prepare
		if (_flip)
		{
			_flipBuf2->enable();
			_taaShader->setTexture("previousLowResTexture", _flipBuf1, "color");
			mainBuffer->render(_taaShader);
		}
		else
		{
			_flipBuf1->enable();
			_taaShader->setTexture("previousLowResTexture", _flipBuf2, "color");
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