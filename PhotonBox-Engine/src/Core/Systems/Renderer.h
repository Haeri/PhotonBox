#ifndef RENDERER_H
#define RENDERER_H

class ObjectRenderer;
class CubeMap;
class FrameBuffer;

#include <vector>
#include "../../Resources/SkyBox.h"
#include "../../Resources/ForwardAmbientLightShader.h"
#include "../../Resources/ForwardDirectionalLightShader.h"
#include "../../Resources/ForwardPointLightShader.h"

class Renderer {
public:
	static bool isDebug() { return _isDebug; }
	static void setDebug(bool debug);
	static void addToRenderQueue(ObjectRenderer *behaviour, bool isOpaque);
	static void removeFromRenderQueue(ObjectRenderer *behaviour);
	static void setSkyBox(CubeMap* cubeMap);
	static SkyBox* getSkyBox() { return &_skyBox; }
	static void printList();
	static FrameBuffer* getMainFrameBuffer() { return _mainFrameBuffer; }
	static void setClearColor(Vector3f color);
	static Vector3f getClearColor() { return _clearColor; }

	void init();
	void start();
	static void render();
	static void render(bool captureMode);
	static void render(Shader* customShader);
	static void renderShadows();
	void destroy();
private:
	static void sortTransparents();
	static SkyBox _skyBox;
	static FrameBuffer* _mainFrameBuffer;
	static bool _isDebug;
	static std::vector<ObjectRenderer*> _renderQueueOpaque;
	static std::map<ObjectRenderer*, float> _renderQueueTransparent;
	static Vector3f _clearColor;

	static ForwardAmbientLightShader* _ambientLightShader;
	static ForwardDirectionalLightShader* _directionalLightShader;
	static ForwardPointLightShader* _pointLightShader;
};

#endif // RENDERER_H
