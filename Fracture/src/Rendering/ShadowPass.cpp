#include "ShadowPass.h"
#include "RenderTarget.h"
#include "DrawCommand.h"
#include "RenderBucket.h"
#include "Material.h"
#include "Shader.h"
#include "Entity/ILight.h"
#include "Entity/SunLight.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Renderer.h"
#include "Component/ICamera.h"
#include "OpenGL/OpenGLBase.h"

Fracture::ShadowPass::ShadowPass()
{
	m_renderTarget = RenderTarget::CreateRenderTarget("ShadowPass",SHADOW_WIDTH,SHADOW_HEIGHT,AttachmentTarget::Texture2D,FormatType::Float,1,true);
}

Fracture::ShadowPass::~ShadowPass()
{
}

void Fracture::ShadowPass::Begin()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	m_renderTarget->bind();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_DEPTH_BUFFER_BIT);	

}

void Fracture::ShadowPass::End()
{
	m_renderTarget->Unbind();
}

void Fracture::ShadowPass::Prepare(std::shared_ptr<SunLight> light)
{
	if (light)
	{
		glm::mat4 lightProjection;
		glm::mat4 lightView;
		lightProjection = glm::ortho(m_left, m_right, m_bottom, m_top, near_plane, far_plane);
		//cameraPosition + (directionalLight.mLightDirection * Z_FAR / 2.0f)
		lightView = glm::lookAt(-light->GetDirection(), glm::vec3(0) , glm::vec3(0.0, 1.0, 0.0));
		m_lightspaceMatrix = lightProjection * lightView;
	}	
}

void Fracture::ShadowPass::Render(std::shared_ptr<Material> material, RenderBucket& bucket)
{
	for (const auto& command : bucket.getShadowRenderCommands())
	{
		material->getShader()->use();
		material->getShader()->setMat4("lightSpaceMatrix", m_lightspaceMatrix);
		material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command.ID)->GetWorldTransform());
		glBindVertexArray(command.VAO);
		glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void Fracture::ShadowPass::Bind()
{
	m_renderTarget->bind();
}

void Fracture::ShadowPass::unBind()
{
	m_renderTarget->Unbind();
}

glm::mat4 Fracture::ShadowPass::GetLightSpaceMatrix()
{	
	return m_lightspaceMatrix;
}

void Fracture::ShadowPass::SetOrthor(float l, float r, float t, float b)
{
	m_left = l;
	m_right = r;
	m_top = t;
	m_bottom = b;
}

void Fracture::ShadowPass::SetNearFarPlanes(float n, float f)
{
	near_plane = n;
	far_plane = f;
}

glm::vec2 Fracture::ShadowPass::GetNearFarPlanes()
{
	return glm::vec2(near_plane,far_plane);
}

glm::vec4 Fracture::ShadowPass::GetOrthor()
{
	return glm::vec4(m_left,m_right,m_top,m_bottom);
}

std::shared_ptr<Fracture::RenderTarget> Fracture::ShadowPass::GetRenderTarget()
{
	return m_renderTarget;
}
