#include "DepthNode.h"

#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "AssetManager/AssetManager.h"
#include "Component/ICamera.h"
#include "Rendering/Material.h"
#include "Profiling/Profiler.h"

Fracture::DepthNode::DepthNode(const std::string& name, const int& width, const int& height, const std::shared_ptr<RenderBucket>& bucket) :
	RenderQueueNode(name,bucket),
	m_shader(AssetManager::getShader("DepthPass"))
{
	std::shared_ptr<OutputSocket> m_output = OutputSocket::Craete("outputDepthMap");

	outputDepthMap = RenderTarget::CreateRenderTarget("DepthPass_out", width, height, AttachmentTarget::Texture2D,FormatType::Float, 1, true);

	AddOutputSocket(m_output);
	AddOutputResource(m_output, outputDepthMap);
}

void Fracture::DepthNode::execute(Renderer& renderer)
{
	ProfilerTimer timer("Depth Pre-Pass");
	resources["outputDepthMap"]->bind();
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	renderer.clear();
	m_shader->use();
	m_shader->setFloat("nearPlane", NearPlane);
	m_shader->setFloat("farPlane", FarPlane);
	
	const auto& forwardRenderCommands = GetBucket()->getForwardRenderCommands();
	const auto& alphaRenderCommands = GetBucket()->getAlphaRenderCommands();

	for (unsigned int i = 0; i < forwardRenderCommands.size(); ++i)
	{
		DrawCommand command = forwardRenderCommands[i];
		renderer.Submit(command, m_shader.get());
	}   
	resources["outputDepthMap"]->Unbind();
}

nlohmann::json Fracture::DepthNode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitDepthNode(*this);
}
