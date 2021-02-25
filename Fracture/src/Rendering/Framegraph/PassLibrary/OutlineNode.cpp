#include "OutlineNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Rendering/OpenGL/OpenGLBase.h"

Fracture::OutlineNode::OutlineNode(const std::string& name, const int& width, const int& height, const std::shared_ptr<RenderBucket>& bucket):
	RenderQueueNode(name,bucket),
	m_firstshader(AssetManager::getShader("PrimitiveMaterial")),
	m_shader(AssetManager::getShader("OutlinePass"))
{
		
	std::shared_ptr<OutputSocket> m_output =  OutputSocket::Craete("outline_out");

	out_resource = RenderTarget::CreateRenderTarget("outline_out",width, height, AttachmentTarget::Texture2D,FormatType::Float, 1, true);
		
	AddOutputSocket(m_output);

	//Link Sockets to Resources	
	AddOutputResource(m_output, out_resource);
}

void Fracture::OutlineNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("Outline-Pass");
	resources["outline_out"]->bind();
	renderer.clear();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);	
	m_firstshader->use();

	const auto& outlineRenderCommands = GetBucket()->getOutlineRenderCommands();
	for (unsigned int i = 0; i < outlineRenderCommands.size(); ++i)
	{
		DrawCommand command = outlineRenderCommands[i];
		renderer.Submit(command, m_shader.get());
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);	
	glDisable(GL_DEPTH_TEST);
	
	m_shader->use();

	for (unsigned int i = 0; i < outlineRenderCommands.size(); ++i)
	{
		DrawCommand command = outlineRenderCommands[i];	
		renderer.Submit(command, m_shader.get());
	}

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	resources["outline_out"]->Unbind();




}

nlohmann::json Fracture::OutlineNode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitOutlineNode(*this);
}
