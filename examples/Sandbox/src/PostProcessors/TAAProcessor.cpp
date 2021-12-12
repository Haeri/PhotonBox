#ifndef TAA_PROCESSOR_CPP
#define TAA_PROCESSOR_CPP

#include <PhotonBox/core/system/Renderer.h>
#include <PhotonBox/resource/PostProcessor.h>
#include <PhotonBox/resource/Material.h>
#include <PhotonBox/resource/FrameBuffer.h>

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

		_taaMaterial = new Material(TAAShader::getInstance());
		_taaMaterial->setImageBuffer("lowResTexture", mainBuffer->getAttachment("color"));
		_taaMaterial->setImageBuffer("previousLowResTexture", _flipBuf1->getAttachment("color"));
		_taaMaterial->setImageBuffer("gPosition", Renderer::getGBuffer()->getAttachment("gPosition3"));
		_taaMaterial->setImageBuffer("normalVelocity", Renderer::getGBuffer()->getAttachment("gVelocity2"));
	}

	void render(FrameBuffer* nextBuffer) override
	{
		// Prepare
		if (_flip)
		{
			_flipBuf2->enable();
			_taaMaterial->setImageBuffer("previousLowResTexture", _flipBuf1->getAttachment("color"));
			mainBuffer->render(_taaMaterial);
		}
		else
		{
			_flipBuf1->enable();
			_taaMaterial->setImageBuffer("previousLowResTexture", _flipBuf2->getAttachment("color"));
			mainBuffer->render(_taaMaterial);
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
		delete _taaMaterial;
		delete _flipBuf1;
		delete _flipBuf2;
	}

private:
	bool _flip = true;
	Material * _taaMaterial;
	FrameBuffer* _flipBuf1, *_flipBuf2;
};

#endif // TAA_PROCESSOR_CPP