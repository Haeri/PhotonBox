#ifndef B_LIGHT_H
#define B_LIGHT_H

#include <memory>
#include "vmmlib/vector.hpp"
#include "Renderer_GL.h"
#include "Configuration.h"


/** @brief A simple point light source
*	@author Benjamin Buergisser
*/
class Light
{
public:

	/* Functions */

	/**	@brief Constructor loading standard values for position, color, intensity and attenuation
	*/
	Light()
		: Light(bRenderer::DEFAULT_LIGHT_POSITION(), bRenderer::DEFAULT_LIGHT_COLOR(), bRenderer::DEFAULT_LIGHT_INTENSITY(), bRenderer::DEFAULT_LIGHT_ATTENUATION(), bRenderer::DEFAULT_LIGHT_RADIUS())
	{}

	/**	@brief Constructor loading standard values for intensity and attenuation
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light for both diffuse and specular lighting
	*/
	Light(const vmml::Vector3f &position, const vmml::Vector3f &color)
		: Light(position, color, bRenderer::DEFAULT_LIGHT_INTENSITY(), bRenderer::DEFAULT_LIGHT_ATTENUATION(), bRenderer::DEFAULT_LIGHT_RADIUS())
	{}

	/**	@brief Constructor
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light for both diffuse and specular lighting
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*	@param[in] radius Radius of the light (clamps the light at a certain distance)
	*/
	Light(const vmml::Vector3f &position, const vmml::Vector3f &color, GLfloat intensity, GLfloat attenuation, GLfloat radius)
		: Light(position, color, color, intensity, attenuation, radius)
	{}

	/**	@brief Constructor
	*	@param[in] position Position of the light
	*	@param[in] diffuseColor Color of the light for diffuse lighting
	*	@param[in] specularColor Color of the light for specular lighting
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*	@param[in] radius Radius of the light (clamps the light at a certain distance)
	*/
	Light(const vmml::Vector3f &position, const vmml::Vector3f &diffuseColor, const vmml::Vector3f &specularColor, GLfloat intensity, GLfloat attenuation, GLfloat radius)
		: _position(vmml::Vector4f(position, 1.0)), _diffuseColor(diffuseColor), _specularColor(specularColor), _intensity(intensity), _attenuation(attenuation), _radius(radius)
	{}

	/**	@brief Virtual destructor
	*/
	virtual ~Light(){}

	/**	@brief Sets the position of the light
	*	@param[in] position Position of the light
	*/
	void setPosition(const vmml::Vector3f &position)	{ _position = vmml::Vector4f(position, 1.0); }

	/**	@brief Sets the color of the light for diffuse lighting
	*	@param[in] color Color of the light for diffuse lighting
	*/
	void setDiffuseColor(const vmml::Vector3f &color)	{ _diffuseColor = color; }

	/**	@brief Sets the color of the light for specular lighting
	*	@param[in] color Color of the light for specular lighting
	*/
	void setSpecularColor(const vmml::Vector3f &color)	{ _specularColor = color; }

	/**	@brief Sets the intensity of the light
	*	@param[in] intensity Intensity of the light
	*/
	void setIntensity(GLfloat intensity)		{ _intensity = intensity; }

	/**	@brief Sets the attenuation of the light
	*	@param[in] attenuation Attenuation of the light
	*/
	void setAttenuation(GLfloat attenuation)	{ _attenuation = attenuation; }

	/**	@brief Sets the radius of the light
	*	@param[in] radius Radius of the light
	*/
	void setRadius(GLfloat radius)		{ _radius = radius; }

	/**	@brief Returns the position of the light
	*/
	const vmml::Vector4f &getPosition()		{ return _position; }

	/**	@brief Returns the color of the light for diffuse lighting
	*/
	const vmml::Vector3f &getDiffuseColor()	{ return _diffuseColor; }

	/**	@brief Returns the color of the light for specular lighting
	*/
	const vmml::Vector3f &getSpecularColor()	{ return _specularColor; }

	/**	@brief Returns the intensity of the light
	*/
	GLfloat getIntensity()		{ return _intensity; }

	/**	@brief Returns the attenuation of the light
	*/
	GLfloat getAttenuation()	{ return _attenuation; }

	/**	@brief Returns the radius of the light
	*/
	GLfloat getRadius()			{ return _radius; }

private:

	/* Variables */

	vmml::Vector4f _position;
	vmml::Vector3f _diffuseColor;
	vmml::Vector3f _specularColor;
	GLfloat _intensity;
	GLfloat _attenuation;
	GLfloat _radius;

};

typedef std::shared_ptr<Light> LightPtr;

#endif /* defined(B_LIGHT_H) */
