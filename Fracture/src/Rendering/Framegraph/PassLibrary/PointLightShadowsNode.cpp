#include "PointLightShadowsNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "Rendering/OpenGL/FrameBuffer.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "Entity/ILight.h"
#include "Entity/PointLight.h"


Fracture::PointShadowsNode::PointShadowsNode(const std::string& name, const int& width,const int& height,std::shared_ptr<RenderBucket> bucket, const std::shared_ptr<PointLight>& light):
	RenderQueueNode(name,bucket),
	Width(width),
	Height(height),
	m_light(light),
	aspect ((float)width / (float)height),
	m_near(1.0f),
	m_far(25.0f)
{
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outShadowMap");

	outShadowMap = RenderTarget::CreateRenderTarget("PointShadows_Out",1024, 1024, AttachmentTarget::CubeMapPosX,FormatType::Float,1,true);
	m_shader = AssetManager::getShader("PointShadows");
	
	AddOutputSocket(m_output);
	AddOutputResource(m_output, outShadowMap);
}

void Fracture::PointShadowsNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("Point Shadow node");
	renderer.setViewport(Width, Height);
	PrepareLightMatrix();

	resources["outShadowMap"]->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shader->use();

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_shader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}

	m_shader->setFloat("far_plane", m_far);
	m_shader->setVec3("lightPos", m_light->GetPosition());

	const auto& shadowRenderCommands = GetBucket()->getShadowRenderCommands();

	for (unsigned int i = 0; i < shadowRenderCommands.size(); ++i)
	{
		DrawCommand command = shadowRenderCommands[i];
		renderer.Submit(command, m_shader.get());
	}

	resources["outShadowMap"]->Unbind();
}

void Fracture::PointShadowsNode::PrepareLightMatrix()
{
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, m_near, m_far);	
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_light->GetPosition(), m_light->GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_light->GetPosition(), m_light->GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_light->GetPosition(), m_light->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_light->GetPosition(), m_light->GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_light->GetPosition(), m_light->GetPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_light->GetPosition(), m_light->GetPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

}
