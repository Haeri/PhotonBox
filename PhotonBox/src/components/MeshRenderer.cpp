#include "PhotonBox/components/MeshRenderer.h"

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/core/systems/Lighting.h"
#include "PhotonBox/core/systems/Renderer.h"
#include "PhotonBox/resources/SkyBox.h"
#include "PhotonBox/resources/Texture.h"
#include "PhotonBox/components/AmbientLight.h"
#include "PhotonBox/components/Camera.h"
#include "PhotonBox/components/DirectionalLight.h"
#include "PhotonBox/components/PointLight.h"
#include "PhotonBox/components/Transform.h"

#ifdef MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void MeshRenderer::render()
{
	render(nullptr, nullptr);
}

void MeshRenderer::render(Shader* shader)
{
	render(shader, nullptr);
}

void MeshRenderer::render(Shader* shader, LightEmitter* light)
{
	if (shader == nullptr) shader = _material->shader;

	glBindVertexArray(_mesh->getVAO());
	shader->bind();

	if (light == nullptr)
		shader->update(transform);
	else
		shader->update(transform, light);

	shader->updateTextures();

	_material->updateUniforms(shader);
	_material->bindTextures(shader);

	shader->enableAttributes();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->getEBO());
	glDrawElements(GL_TRIANGLES, (GLsizei)_mesh->indices.size(), GL_UNSIGNED_INT, 0);
	Renderer::addDrawCall();

	shader->disableAttributes();

	glBindVertexArray(0);
}

BoundingSphere MeshRenderer::getBoundingSphere()
{
	return _mesh->boundingSphere;
}

MeshRenderer & MeshRenderer::setMesh(Mesh * mesh)
{
	_mesh = mesh;
	return *this;
}
