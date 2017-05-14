#ifndef B_MODEL_RENDERER_H
#define B_MODEL_RENDERER_H

/* bRenderer includes */
#include "Logger.h"
#include "ObjectManager.h"
#include "RenderQueue.h"

/* vmmlib includes */
#include "vmmlib/util.hpp"
#include "vmmlib/frustum_culler.hpp"


/** @brief A helper class to efficiently draw and queue models with one function call
*	@author Benjamin Buergisser
*/
class ModelRenderer
{
public:

	/* Functions */

	/**	@brief Constructor
	*/
	ModelRenderer();

	/**	@brief Constructor
	*	@param[in] o Pointer to the object manager
	*/
	ModelRenderer(ObjectManagerPtr o);

	/**	@brief Virtual destructor
	*/
	virtual ~ModelRenderer(){}

	/**	@brief Returns a pointer to the object manager used to get the models, cameras and lights from
	*/
	ObjectManagerPtr getObjectManager();

	/**	@brief Set the object manager to get the models, cameras and lights from
	*	@param[in] o Pointer to the object manager
	*/
	void setObjectManager(ObjectManagerPtr o);

	/**	@brief Returns a pointer to the render queue
	*/
	RenderQueuePtr getRenderQueue();

	/**	@brief Draw specified model into the current framebuffer
	*	@param[in] modelName Name of the model
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*/
	virtual void drawModel(const std::string &modelName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false);

	/**	@brief Draw specified model into the current framebuffer
	*	@param[in] model The model to be drawn
	*	@param[in] modelMatrix
	*	@param[in] viewMatrix
	*	@param[in] projectionMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*/
	virtual void drawModel(ModelPtr model, const vmml::Matrix4f &modelMatrix, const vmml::Matrix4f &viewMatrix, const vmml::Matrix4f &projectionMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false);

	/**	@brief Queue specified model into the render queue
	*	@param[in] modelName Name of the model
	*	@param[in] instanceName The name of the model instance to be queued (instance is created automatically if not present)
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*	@param[in] isTransparent Set true if the model is (partially) transparent and sorting according to distance should occur (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*	@param[in] customDistance If the function should not determine the distance to the camera a custom distance can be set (optional)
	*/
	virtual void queueModelInstance(const std::string &modelName, const std::string &instanceName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA, GLfloat customDistance = 10000.0f);

	/**	@brief Queue specified model into the render queue
	*	@param[in] model The model to be drawn
	*	@param[in] instanceName The name of the model instance to be queued (instance is created automatically if not present)
	*	@param[in] modelMatrix
	*	@param[in] viewMatrix
	*	@param[in] projectionMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the model should be tested against the view frustum (optional)
	*	@param[in] cullIndividualGeometry Set true if all the geometry should be tested against the view frustum (optional)
	*	@param[in] isTransparent Set true if the model is (partially) transparent and sorting according to distance should occur (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*	@param[in] customDistance If the function should not determine the distance to the camera a custom distance can be set (optional)
	*/
	virtual void queueModelInstance(ModelPtr model, const std::string &instanceName, const vmml::Matrix4f &modelMatrix, const vmml::Matrix4f &viewMatrix, const vmml::Matrix4f &projectionMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, bool cullIndividualGeometry = false, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA, GLfloat customDistance = 10000.0f);

	/**	@brief Draw specified text sprite into the current framebuffer
	*	@param[in] textSpriteName Name of the text sprite
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the text sprite should be tested against the view frustum (optional)
	*/
	virtual void drawText(const std::string &textSpriteName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = false);

	/**	@brief Queue specified text sprite into the render queue
	*	@param[in] textSpriteName Name of the text sprite
	*	@param[in] instanceName The name of the text sprite instance to be queued (instance is created automatically if not present)
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix
	*	@param[in] lightNames Names of the lights in a vector
	*	@param[in] doFrustumCulling Set true if the text sprite should be tested against the view frustum (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*	@param[in] customDistance If the function should not determine the distance to the camera a custom distance can be set (optional)
	*/
	virtual void queueTextInstance(const std::string &textSpriteName, const std::string &instanceName, const std::string &cameraName, const vmml::Matrix4f &modelMatrix, const std::vector<std::string> &lightNames, bool doFrustumCulling = true, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA, GLfloat customDistance = 10000.0f);

	/**	@brief Draws the render queue into the current framebuffer
	*	@param[in] mode
	*/
	void drawQueue(GLenum mode = GL_TRIANGLES);

	/**	@brief Clears the render queue
	*/
	void clearQueue();

	/**	@brief Tests an axis-aligned bounding box against the view frustum
	*	@param[in] aabbObjectSpace The axis-aligned bounding box in object space
	*	@param[in] modelViewProjectionMatrix The model view projection matrix (projection * view * model)
	*/
	vmml::Visibility viewFrustumCulling(const vmml::AABBf &aabbObjectSpace, const vmml::Matrix4f &modelViewProjectionMatrix);

private:

	/* Variables */

	ObjectManagerPtr	_objectManager = nullptr;
	RenderQueuePtr		_renderQueue = nullptr;
};

typedef std::shared_ptr< ModelRenderer >  ModelRendererPtr;

#endif /* defined(B_MODEL_RENDERER_H) */
