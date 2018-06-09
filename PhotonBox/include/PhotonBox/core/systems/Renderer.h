#ifndef RENDERER_H
#define RENDERER_H

class CubeMap;
class FrameBuffer;
class TransparentShader;
class GShader;
class TransparentMeshRenderer;
class DeferredShader;
class Material;

#include <vector>

#include "PhotonBox/resources/ForwardAmbientLightShader.h"
#include "PhotonBox/resources/ForwardDirectionalLightShader.h"
#include "PhotonBox/resources/ForwardPointLightShader.h"
#include "PhotonBox/resources/ForwardSpotLightShader.h"
#include "PhotonBox/resources/SkyBox.h"

enum RenderType
{
	opaque, cutout, transparent
};

class Renderer
{
public:
	static const int MAX_DEBUG = 4;

	static int getDebugMode() { return _debugMode; }
	static void setDebug(int debugMode);
	static void addToRenderQueue(ObjectRenderer *renderer, RenderType type);
	static void removeFromRenderQueue(ObjectRenderer *renderer);
	static void setSkyBox(CubeMap* cubeMap);
	static SkyBox* getSkyBox() { return &_skyBox; }
	static void printList();
	static void setClearColor(Vector3f color);
	static Vector3f getClearColor() { return _clearColor; }
	static void addDrawCall();
	static int getDrawCalls() { return _drawCalls; }
	static FrameBuffer* getMainFrameBuffer() { return _mainFrameBuffer; }
	static FrameBuffer* getGBuffer() { return _gBuffer; }
	static FrameBuffer* getDebugBuffer() { return _gizmoBuffer; }

	void init();
	void init(float superSampling);
	void start();
	void prePass();
	void clearDrawCalls();
	static void renderBase();
	static void renderTransparents();
	static void renderDeferred();
	static void render();
	static void render(bool captureMode);
	static void render(Shader* customShader, bool captureMode);
	static void render(bool captureMode, LightMap* lightmap);
	static void renderShadows(bool captureMode);
	static void renderAmbient(int pass, LightMap* lightMap, AABB* volume);
	void renderGizmos();
	void destroy();
private:
	static int _drawCalls;
	static void clearTransparentQueue();
	static void updateTransparentQueue();
	static SkyBox _skyBox;
	static FrameBuffer* _mainFrameBuffer;
	static FrameBuffer* _gBuffer;
	static FrameBuffer*	_gizmoBuffer;
	static int _debugMode;
	static std::vector<ObjectRenderer*> _renderListOpaque;
	static std::vector<ObjectRenderer*> _renderListTransparent;
	static Vector3f _clearColor;

	static std::map<float, TransparentMeshRenderer*> _renderQueueTransparent;

	static ForwardAmbientLightShader* _ambientLightShader;
	static ForwardDirectionalLightShader* _directionalLightShader;
	static ForwardPointLightShader* _pointLightShader;
	static ForwardSpotLightShader* _spotLightShader;
	static TransparentShader* _transparentBaseShader;
	static GShader* _gShader;
	static DeferredShader* _deferredShader;
	static Material* _deferredMaterial;
};

#endif // RENDERER_H
