#include "PhotonBox/component/MeshRenderer.h"

#include "PhotonBox/core/Entity.h"
#include "PhotonBox/core/OpenGL.h"
#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/resource/SkyBox.h"
#include "PhotonBox/resource/Texture.h"
#include "PhotonBox/component/AmbientLight.h"
#include "PhotonBox/component/Camera.h"
#include "PhotonBox/component/DirectionalLight.h"
#include "PhotonBox/component/PointLight.h"
#include "PhotonBox/component/Transform.h"
#include "PhotonBox/resource/Material.h"
#include "PhotonBox/resource/Mesh.h"

#ifdef PB_MEM_DEBUG
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
	if (shader == nullptr) shader = _material->getShader();

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
