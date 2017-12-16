#ifndef RENDERER_H
#define RENDERER_H

class ObjectRenderer;
class CubeMap;
class FrameBuffer;
class TransparentShader;

#include <vector>
#include "../../Resources/SkyBox.h"
#include "../../Resources/ForwardAmbientLightShader.h"
#include "../../Resources/ForwardDirectionalLightShader.h"
#include "../../Resources/ForwardPointLightShader.h"
#include "../../Resources/ForwardSpotLightShader.h"

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
	void init(int superSampling);
	void start();
	static void render();
	static void render(bool captureMode);
	static void render(Shader* customShader, bool captureMode);
	static void render(bool captureMode, LightMap* lightmap);
	static void renderShadows(bool captureMode);
	static void renderAmbient(int pass, LightMap* lightMap, AABB* volume);
	void renderGizmos();
	void destroy();
private:
	static void clearTransparentQueue();
	static void updateTransparentQueue();
	static SkyBox _skyBox;
	static FrameBuffer* _mainFrameBuffer;
	static bool _isDebug;
	static std::vector<ObjectRenderer*> _renderListOpaque;
	static std::vector<ObjectRenderer*> _renderListTransparent;
	static Vector3f _clearColor;

	static std::map<float, ObjectRenderer*> _renderQueueTransparent;

	static ForwardAmbientLightShader* _ambientLightShader;
	static ForwardDirectionalLightShader* _directionalLightShader;
	static ForwardPointLightShader* _pointLightShader;
	static ForwardSpotLightShader* _spotLightShader;
	static TransparentShader* _transparentBaseShader;
};

#endif // RENDERER_H
