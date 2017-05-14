#include "headers/RenderQueue.h"

void RenderQueue::submitToRenderQueue(GLuint programID, const std::string &materialName, const std::string &drawableName, const std::string &instanceName, DrawablePtr drawable, GLfloat distanceToCamera, bool isTransparent, GLenum blendSfactor, GLenum blendDfactor)
{
	// Transparent
	if (isTransparent){
		_renderCallsTransparent.insert(RenderCallsTransparent::value_type(distanceToCamera, RenderCall{ drawable, instanceName, blendSfactor, blendDfactor }));
	}
	// Opaque
	else{
		std::string key = std::to_string(programID)+materialName + drawableName;
		_renderCallsOpaque.insert(RenderCallsOpaque::value_type(key, RenderCall{ drawable, instanceName }));
	}
}

void RenderQueue::draw(GLenum mode)
{
	// Opaque
	for (auto renderCall = _renderCallsOpaque.begin(); renderCall != _renderCallsOpaque.end(); ++renderCall){
		renderCall->second.drawable->drawInstance((renderCall->second.instanceName), mode);
	}

	// Transparent
	for (auto renderCall = _renderCallsTransparent.rbegin(); renderCall != _renderCallsTransparent.rend(); ++renderCall){
		glBlendFunc(renderCall->second.blendSfactor, renderCall->second.blendDfactor);
		renderCall->second.drawable->drawInstance((renderCall->second.instanceName), mode);
	}

	// reset blend function so opaque objects are not affected
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderQueue::clear()
{
	// Opaque
	_renderCallsOpaque.clear();

	// Transparent
	_renderCallsTransparent.clear();
}