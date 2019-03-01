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
	bool flip = true;

	TAAProcessor(int index) : PostProcessor(index)
	{
		_mainBuffer = new FrameBuffer(1);
		_mainBuffer->addTextureAttachment("color", true);
		_mainBuffer->ready();

		_flipBuf1 = new FrameBuffer(1);
		_flipBuf1->addTextureAttachment("color", true);
		_flipBuf1->ready();

		_flipBuf2 = new FrameBuffer(1);
		_flipBuf2->addTextureAttachment("color", true);
		_flipBuf2->ready();

		_taaShader = new Material(TAAShader::getInstance());
		_taaShader->setTexture("lowResTexture", _mainBuffer, "color");
		_taaShader->setTexture("gPosition", Renderer::getGBuffer(), "gPosition");
		_taaShader->setTexture("normalVelocity", Renderer::getGBuffer(), "gMetallic");
	}

	void prepare() override
	{
		_mainBuffer->enable();
	}

	void preProcess() override
	{
		if (flip)
		{
			_flipBuf2->enable();
			_taaShader->setTexture("previousLowResTexture", _flipBuf1, "color");
			_mainBuffer->render(_taaShader);
		}
		else
		{
			_flipBuf1->enable();
			_taaShader->setTexture("previousLowResTexture", _flipBuf2, "color");
			_mainBuffer->render(_taaShader);
		}
	}


	void render() override
	{
		if (flip)
		{
			_flipBuf2->render("color");
		}
		else
		{
			_flipBuf1->render("color");
		}

		flip = !flip;
	}

	void destroy() override
	{
		delete _taaShader;
		delete _mainBuffer;
		delete _flipBuf1;
		delete _flipBuf2;
	}

private:
	Material * _taaShader;
	FrameBuffer* _mainBuffer, *_flipBuf1, *_flipBuf2;
};

#endif // TAA_PROCESSOR_CPP