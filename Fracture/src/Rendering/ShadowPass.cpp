#include "ShadowPass.h"
#include "RenderTarget.h"
#include "Material.h"
#include "Component/ILight.h"

Fracture::ShadowPass::ShadowPass()
{
	m_renderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(SHADOW_WIDTH, SHADOW_HEIGHT, GL_FLOAT, 0, true));
}

Fracture::ShadowPass::~ShadowPass()
{
}

void Fracture::ShadowPass::Begin()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	m_renderTarget->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_TEXTURE5);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Fracture::ShadowPass::End()
{	
	glBindTexture(GL_TEXTURE_2D, 0);
	m_renderTarget->Unbind();
}

void Fracture::ShadowPass::Prepare(std::shared_ptr<SunLight> light)
{
	if (light->GetLightType() == LightType::Sun)
	{
		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightProjection;
		glm::mat4 lightView;
		lightProjection = glm::ortho(m_left, m_right, m_bottom, m_top, near_plane, far_plane);
		lightView = glm::lookAt(-light->GetDirection(), glm::vec3(0.0f), glm::vec3(0.0, -1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		m_lightspaceMatrix = lightSpaceMatrix;
	}	
}

void Fracture::ShadowPass::Render()
{
}

void Fracture::ShadowPass::Bind()
{
}

glm::mat4 Fracture::ShadowPass::GetLightSpaceMatrix()
{
	return m_lightspaceMatrix;
}

void Fracture::ShadowPass::SetOrthor(float l, float r, float t, float b)
{
}

void Fracture::ShadowPass::SetNearFarPlanes(float n, float f)
{
}

std::shared_ptr<Fracture::RenderTarget> Fracture::ShadowPass::GetRenderTarget()
{
	return m_renderTarget;
}
